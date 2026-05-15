#pragma once

#include "pb/core/meta.hpp"

namespace pb::core {

namespace detail {

template <typename From, typename... Stages>
struct pipeline_state {
    using input_type = From;
    using stages = meta::type_list<Stages...>;

    template <typename Stage>
    struct then_builder {
        using chain = pipeline_state<From, Stages..., Stage>;

        template <typename Next>
        using to = pipeline_state<From, Stages..., Stage, Next>;
    };

    template <typename Stage>
    using then = then_builder<Stage>;
};

}  // namespace detail

template <typename From>
struct from {
    using source = From;

    template <typename Stage>
    using then = detail::pipeline_state<From>::template then_builder<Stage>;
};

}  // namespace pb::core
