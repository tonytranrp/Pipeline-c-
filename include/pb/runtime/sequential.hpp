#pragma once

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <pb/runtime/result.hpp>

namespace pb::runtime {

namespace detail {

template <class T>
struct unwrap_stage_output {
    using type = T;
};

template <class T, class E>
struct unwrap_stage_output<expected<T, E>> {
    using type = T;
};

template <class T>
using unwrap_stage_output_t = typename unwrap_stage_output<std::remove_cvref_t<T>>::type;

template <class T>
struct is_result_output : std::false_type {};

template <class T>
struct is_result_output<result<T>> : std::true_type {};

template <class T>
inline constexpr bool is_result_output_v = is_result_output<std::remove_cvref_t<T>>::value;

template <class Input, class... Stages>
struct pipeline_output;

template <class Input>
struct pipeline_output<Input> {
    using type = Input;
};

template <class Input, class Stage, class... Rest>
struct pipeline_output<Input, Stage, Rest...> {
    using stage_result = std::invoke_result_t<Stage&, Input>;
    using next_input = unwrap_stage_output_t<stage_result>;

    static_assert(sizeof...(Rest) == 0 || !std::is_void_v<next_input>,
                  "pb::runtime::sequential: void-producing stage may only terminate a pipeline");

    using type = typename pipeline_output<next_input, Rest...>::type;
};

template <std::size_t I, class Final, class Current, class... Stages>
[[nodiscard]] auto run_steps(std::tuple<Stages...>& stages, Current&& current) -> result<Final> {
    if constexpr (I == sizeof...(Stages)) {
        if constexpr (std::is_void_v<Final>) {
            return result<void>{};
        } else {
            return result<Final>(std::forward<Current>(current));
        }
    } else {
        auto& stage = std::get<I>(stages);
        auto next = std::invoke(stage, std::forward<Current>(current));

        if constexpr (is_result_output_v<decltype(next)>) {
            if (!next) {
                return result<Final>(make_unexpected(next.error()));
            }

            using next_value = result_value_t<decltype(next)>;
            if constexpr (std::is_void_v<next_value>) {
                static_assert(I + 1 == sizeof...(Stages),
                              "pb::runtime::sequential: expected<void> stage may only terminate a pipeline");
                return result<Final>(make_unexpected(error{error_code::invalid_input,
                                                          "void stage output cannot feed a non-terminal stage"}));
            } else {
                if constexpr (I + 1 == sizeof...(Stages)) {
                    if constexpr (std::is_void_v<Final>) {
                        return result<void>{};
                    } else {
                        return result<Final>(std::move(next).value());
                    }
                } else {
                    return run_steps<I + 1, Final>(stages, std::move(next).value());
                }
            }
        } else {
            if constexpr (std::is_void_v<decltype(next)>) {
                static_assert(I + 1 == sizeof...(Stages),
                              "pb::runtime::sequential: void-producing stage may only terminate a pipeline");
                return result<Final>(make_unexpected(error{error_code::invalid_input,
                                                          "void stage output cannot feed a non-terminal stage"}));
            } else {
                if constexpr (I + 1 == sizeof...(Stages)) {
                    if constexpr (std::is_void_v<Final>) {
                        return result<void>{};
                    } else {
                        return result<Final>(std::move(next));
                    }
                } else {
                    return run_steps<I + 1, Final>(stages, std::move(next));
                }
            }
        }
    }
}

}  // namespace detail

template <class... Stages>
class linear_pipeline {
public:
    using stages_type = std::tuple<Stages...>;
    static constexpr std::size_t stage_count = sizeof...(Stages);

    constexpr linear_pipeline() requires (std::is_default_constructible_v<Stages> && ...) = default;
    constexpr explicit linear_pipeline(Stages... stages) : stages_(std::move(stages)...) {}

    [[nodiscard]] constexpr auto stages() & -> stages_type& { return stages_; }
    [[nodiscard]] constexpr auto stages() const& -> const stages_type& { return stages_; }

    template <class Input>
    [[nodiscard]] constexpr auto run(Input&& input) &
        -> result<typename detail::pipeline_output<std::remove_cvref_t<Input>, Stages...>::type> {
        using output_type = typename detail::pipeline_output<std::remove_cvref_t<Input>, Stages...>::type;
        return detail::run_steps<0, output_type>(stages_, std::forward<Input>(input));
    }

    template <class Input>
    [[nodiscard]] constexpr auto run(Input&& input) const&
        -> result<typename detail::pipeline_output<std::remove_cvref_t<Input>, Stages...>::type> {
        auto copy = *this;
        return copy.run(std::forward<Input>(input));
    }

private:
    stages_type stages_{};
};

template <class... Stages>
linear_pipeline(Stages...) -> linear_pipeline<std::decay_t<Stages>...>;

template <class... Stages>
[[nodiscard]] constexpr auto make_linear_pipeline(Stages&&... stages) -> linear_pipeline<std::decay_t<Stages>...> {
    return linear_pipeline<std::decay_t<Stages>...>(std::forward<Stages>(stages)...);
}

class sequential {
public:
    constexpr sequential() = default;

    template <class Pipeline, class Input>
    [[nodiscard]] constexpr auto run(Pipeline&& pipeline, Input&& input) const
        -> decltype(std::forward<Pipeline>(pipeline).run(std::forward<Input>(input))) {
        return std::forward<Pipeline>(pipeline).run(std::forward<Input>(input));
    }

    template <class Pipeline, class Input>
    [[nodiscard]] constexpr auto operator()(Pipeline&& pipeline, Input&& input) const
        -> decltype(std::forward<Pipeline>(pipeline).run(std::forward<Input>(input))) {
        return run(std::forward<Pipeline>(pipeline), std::forward<Input>(input));
    }
};

}  // namespace pb::runtime
