#include <pb/pipeline.hpp>
#include <pb/adapt/sender_receiver.hpp>

#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace {
void pb_test_require(bool condition) {
  if (!condition) {
    std::abort();
  }
}

struct In {
  int value{};
};

struct Mid {
  int value{};
};

struct Out {
  int value{};
};

struct MakeMidSender {
  using output_type = Mid;

  auto operator()(In input) const {
    return pb::sync_just(Mid{input.value * 2});
  }
};

struct Finish {
  using input_type = Mid;
  using output_type = Out;

  static constexpr auto stage_name() noexcept { return "finish"; }

  Out operator()(Mid mid) const { return Out{mid.value + 3}; }
};

using SenderStage = pb::sync_sender_stage<MakeMidSender, In>;
using Pipeline = pb::from<In>::then<SenderStage>::then<Finish>::to<Out>;

static_assert(pb::core::Stage<SenderStage>);
static_assert(std::is_same_v<SenderStage::input_type, In>);
static_assert(std::is_same_v<SenderStage::output_type, Mid>);
static_assert(pb::valid<Pipeline>);

struct ErrorSenderFactory {
  using output_type = Mid;

  struct sender {
    template <class Receiver>
    struct operation {
      Receiver receiver;
      void start() { receiver.set_error(std::make_exception_ptr(std::runtime_error{"sender boom"})); }
    };

    template <class Receiver>
    auto connect(Receiver receiver) && -> operation<std::remove_cvref_t<Receiver>> {
      return operation<std::remove_cvref_t<Receiver>>{std::move(receiver)};
    }
  };

  auto operator()(In) const { return sender{}; }
};

struct StoppedSenderFactory {
  using output_type = Mid;

  struct sender {
    template <class Receiver>
    struct operation {
      Receiver receiver;
      void start() noexcept { receiver.set_stopped(); }
    };

    template <class Receiver>
    auto connect(Receiver receiver) && -> operation<std::remove_cvref_t<Receiver>> {
      return operation<std::remove_cvref_t<Receiver>>{std::move(receiver)};
    }
  };

  auto operator()(In) const { return sender{}; }
};

struct EmptySenderFactory {
  using output_type = Mid;

  struct sender {
    template <class Receiver>
    struct operation {
      Receiver receiver;
      void start() noexcept { (void)receiver; }
    };

    template <class Receiver>
    auto connect(Receiver receiver) && -> operation<std::remove_cvref_t<Receiver>> {
      return operation<std::remove_cvref_t<Receiver>>{std::move(receiver)};
    }
  };

  auto operator()(In) const { return sender{}; }
};

} // namespace

int main() {
  // Direct value-sender usage: connect + start completes synchronously.
  struct CaptureReceiver {
    Mid* out;
    void set_value(Mid value) { *out = value; }
    void set_error(std::exception_ptr) noexcept { std::abort(); }
    void set_stopped() noexcept { std::abort(); }
  };

  Mid captured{};
  auto op = pb::sync_just(Mid{7}).connect(CaptureReceiver{&captured});
  op.start();
  pb_test_require(captured.value == 7);

  // Pipeline usage: sender stage unwraps set_value and composes with a normal stage.
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  auto output = engine.run(In{4});
  pb_test_require(output.value == 11);  // (4 * 2) + 3

  auto result = engine.try_run(In{5});
  pb_test_require(result.has_value());
  pb_test_require(result.value().value == 13);

  // set_error becomes the sequential runtime's normal exception-category error.
  using ErrorPipeline = pb::from<In>::then<pb::sync_sender_stage<ErrorSenderFactory, In>>::to<Mid>;
  auto error_engine = pb::compile<ErrorPipeline>(pb::runtime::sequential{});
  auto error_result = error_engine.try_run(In{1});
  pb_test_require(!error_result.has_value());
  pb_test_require(error_result.error().category == pb::runtime::error_category::exception);
  pb_test_require(error_result.error().stage.name == "sync_sender_stage");

  // set_stopped and no-completion are explicit exceptions, not silent defaults.
  using StoppedPipeline = pb::from<In>::then<pb::sync_sender_stage<StoppedSenderFactory, In>>::to<Mid>;
  auto stopped_engine = pb::compile<StoppedPipeline>(pb::runtime::sequential{});
  auto stopped_result = stopped_engine.try_run(In{1});
  pb_test_require(!stopped_result.has_value());
  pb_test_require(stopped_result.error().category == pb::runtime::error_category::exception);

  using EmptyPipeline = pb::from<In>::then<pb::sync_sender_stage<EmptySenderFactory, In>>::to<Mid>;
  auto empty_engine = pb::compile<EmptyPipeline>(pb::runtime::sequential{});
  auto empty_result = empty_engine.try_run(In{1});
  pb_test_require(!empty_result.has_value());
  pb_test_require(empty_result.error().category == pb::runtime::error_category::exception);

  return 0;
}
