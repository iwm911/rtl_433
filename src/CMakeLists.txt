########################################################################
# Build libraries and executables
########################################################################
# commodity libraries
# consider -fvisibility=hidden
# Proper object library type was only introduced with CMake 2.8.8
add_library(r_433 STATIC
    abuf.c
    am_analyze.c
    baseband.c
    bit_util.c
    bitbuffer.c
    compat_paths.c
    compat_time.c
    confparse.c
    data.c
    data_tag.c
    decoder_util.c
    fileformat.c
    http_server.c
    jsmn.c
    list.c
    logger.c
    mongoose.c
    optparse.c
    output_file.c
    output_influx.c
    output_log.c
    output_mqtt.c
    output_rtltcp.c
    output_trigger.c
    output_udp.c
    pulse_analyzer.c
    pulse_data.c
    pulse_detect.c
    pulse_detect_fsk.c
    pulse_slicer.c
    r_api.c
    r_util.c
    raw_output.c
    rfraw.c
    samp_grab.c
    sdr.c
    term_ctl.c
    write_sigrok.c
    devices/flex.c
    devices/generic_motion.c
    devices/generic_remote.c
    devices/generic_temperature_sensor.c
    devices/new_template.c
    devices/tpms_abarth124.c
    devices/tpms_ave.c
    devices/tpms_bmw.c
    devices/tpms_bmw_g3.c
    devices/tpms_citroen.c
    devices/tpms_eezrv.c
    devices/tpms_elantra2012.c
    devices/tpms_ford.c
    devices/tpms_gm.c
    devices/tpms_hyundai_vdo.c
    devices/tpms_jansite.c
    devices/tpms_jansite_solar.c
    devices/tpms_kia.c
    devices/tpms_nissan.c
    devices/tpms_pmv107j.c
    devices/tpms_porsche.c
    devices/tpms_renault.c
    devices/tpms_renault_0435r.c
    devices/tpms_toyota.c
    devices/tpms_truck.c
    devices/tpms_tyreguard400.c
)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    # untouched upstream code, disable all warnings
    set_source_files_properties(mongoose.c PROPERTIES COMPILE_FLAGS "-w")
endif()

add_executable(rtl_433 rtl_433.c)
target_link_libraries(rtl_433 r_433)

# target_compile_definitions was only added with CMake 2.8.11
if(THREADS_HAVE_PTHREAD_ARG)
    set_target_properties(r_433 PROPERTIES COMPILE_OPTIONS "-pthread")
    set_target_properties(r_433 PROPERTIES INTERFACE_COMPILE_OPTIONS "-pthread")
endif()
if(CMAKE_THREAD_LIBS_INIT)
    target_link_libraries(rtl_433 "${CMAKE_THREAD_LIBS_INIT}")
endif()

if(MSVC)
    # needs CMake 3.1 but Windows builds should have that
    target_sources(rtl_433 PRIVATE getopt/getopt.c)
endif()

add_library(data data.c abuf.c)
target_link_libraries(data ${NET_LIBRARIES})

target_link_libraries(rtl_433
    ${SDR_LIBRARIES}
    ${NET_LIBRARIES}
)

set(INSTALL_TARGETS rtl_433)
if(UNIX)
target_link_libraries(rtl_433 m)
endif()

# Explicitly say that we want C99
set_target_properties(rtl_433 r_433 PROPERTIES C_STANDARD 99)

########################################################################
# Install built library files & utilities
########################################################################
install(TARGETS ${INSTALL_TARGETS}
    RUNTIME DESTINATION bin              # .dll file
)
