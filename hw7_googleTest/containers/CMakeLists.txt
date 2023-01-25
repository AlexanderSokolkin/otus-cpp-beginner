set(HEADERS
        list_container.h
        sequence_container.h)

target_sources(containers_tests
        PRIVATE
                ${HEADERS})
