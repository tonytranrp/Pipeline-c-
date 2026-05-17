#pragma once

#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "pb/runtime/error.hpp"
#include "pb/runtime/observer.hpp"

namespace pb::runtime {

inline constexpr std::string_view trace_schema_version = "pb.runtime.trace.v1";

enum class trace_event_kind {
  stage_start,
  stage_success,
  stage_failure,
  stage_exception,
};

struct trace_event {
  trace_event_kind kind{trace_event_kind::stage_start};
  stage_id stage{};
  bool has_error{false};
  error diagnostic{};
};

struct trace_sink {
  virtual ~trace_sink() = default;
  virtual void on_trace_event(const trace_event& event) = 0;
};

class trace_recorder final : public trace_sink {
public:
  void on_trace_event(const trace_event& event) override { events_.push_back(event); }

  void clear() noexcept { events_.clear(); }

  [[nodiscard]] auto empty() const noexcept -> bool { return events_.empty(); }
  [[nodiscard]] auto size() const noexcept -> std::size_t { return events_.size(); }

  [[nodiscard]] auto events() const noexcept -> const std::vector<trace_event>& { return events_; }

private:
  std::vector<trace_event> events_{};
};

class trace_observer final : public observer {
public:
  explicit trace_observer(trace_sink& sink) noexcept : sink_(&sink) {}

  void set_sink(trace_sink* sink) noexcept { sink_ = sink; }

  [[nodiscard]] auto get_sink() const noexcept -> trace_sink* { return sink_; }

  void on_stage_start(const stage_id& stage) override {
    emit(trace_event{.kind = trace_event_kind::stage_start, .stage = stage});
  }

  void on_stage_success(const stage_id& stage) override {
    emit(trace_event{.kind = trace_event_kind::stage_success, .stage = stage});
  }

  void on_stage_failure(const stage_id& stage, const error& diagnostic) override {
    emit(trace_event{.kind = trace_event_kind::stage_failure,
                     .stage = stage,
                     .has_error = true,
                     .diagnostic = diagnostic});
  }

  void on_stage_exception(const stage_id& stage, const error& diagnostic) override {
    emit(trace_event{.kind = trace_event_kind::stage_exception,
                     .stage = stage,
                     .has_error = true,
                     .diagnostic = diagnostic});
  }

private:
  void emit(const trace_event& event) {
    if (sink_ != nullptr) {
      sink_->on_trace_event(event);
    }
  }

  trace_sink* sink_{nullptr};
};

[[nodiscard]] inline auto trace_event_kind_name(trace_event_kind kind) noexcept -> std::string_view {
  switch (kind) {
  case trace_event_kind::stage_start:
    return "stage_start";
  case trace_event_kind::stage_success:
    return "stage_success";
  case trace_event_kind::stage_failure:
    return "stage_failure";
  case trace_event_kind::stage_exception:
    return "stage_exception";
  }
  return "unknown";
}

namespace detail {

inline void append_json_string(std::ostream& stream, std::string_view value) {
  stream << '"';
  for (const auto ch : value) {
    switch (ch) {
    case '"':
      stream << "\\\"";
      break;
    case '\\':
      stream << "\\\\";
      break;
    case '\b':
      stream << "\\b";
      break;
    case '\f':
      stream << "\\f";
      break;
    case '\n':
      stream << "\\n";
      break;
    case '\r':
      stream << "\\r";
      break;
    case '\t':
      stream << "\\t";
      break;
    default:
      stream << ch;
      break;
    }
  }
  stream << '"';
}

inline void append_stage_json(std::ostream& stream, const stage_id& stage) {
  stream << "{\"key\":";
  append_json_string(stream, stage.key);
  stream << ",\"name\":";
  append_json_string(stream, stage.name);
  stream << '}';
}

inline void append_error_json(std::ostream& stream, const error& diagnostic) {
  stream << "{\"stage\":";
  append_stage_json(stream, diagnostic.stage);
  stream << ",\"category\":";
  append_json_string(stream, category_name(diagnostic.category));
  stream << ",\"message\":";
  append_json_string(stream, diagnostic.message);
  stream << '}';
}

inline void append_trace_event_json(std::ostream& stream, const trace_event& event) {
  stream << "{\"kind\":";
  append_json_string(stream, trace_event_kind_name(event.kind));
  stream << ",\"stage\":";
  append_stage_json(stream, event.stage);
  if (event.has_error) {
    stream << ",\"error\":";
    append_error_json(stream, event.diagnostic);
  }
  stream << '}';
}

} // namespace detail

[[nodiscard]] inline auto export_json(std::span<const trace_event> events) -> std::string {
  std::ostringstream stream;
  stream << "{\"schema_version\":";
  detail::append_json_string(stream, trace_schema_version);
  stream << ",\"events\":[";
  for (std::size_t index = 0; index < events.size(); ++index) {
    if (index != 0) {
      stream << ',';
    }
    detail::append_trace_event_json(stream, events[index]);
  }
  stream << "]}";
  return stream.str();
}

[[nodiscard]] inline auto export_json(const trace_recorder& recorder) -> std::string {
  return export_json(std::span<const trace_event>{recorder.events().data(), recorder.events().size()});
}

} // namespace pb::runtime
