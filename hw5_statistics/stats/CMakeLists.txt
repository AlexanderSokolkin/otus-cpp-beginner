set(HEADERS
        max_statistic.h
        mean_statistic.h
        min_statistic.h
        pct_statistic.h
        std_statistic.h
        statistic_interface.h)

set(SOURCES
        max_statistic.cpp
        mean_statistic.cpp
        min_statistic.cpp
        pct_statistic.cpp
        std_statistic.cpp)

target_sources(statistics
        PRIVATE
                ${HEADERS}
                ${SOURCES})
