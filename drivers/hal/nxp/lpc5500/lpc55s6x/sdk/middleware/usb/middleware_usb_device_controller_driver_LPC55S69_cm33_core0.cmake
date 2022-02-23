include_guard(GLOBAL)
message("middleware_usb_device_controller_driver component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device/usb_device_dci.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/device
)


#OR Logic component
if(CONFIG_USE_middleware_usb_device_khci_LPC55S69_cm33_core0)
     include(middleware_usb_device_khci_LPC55S69_cm33_core0)
endif()
if(CONFIG_USE_middleware_usb_device_ehci_LPC55S69_cm33_core0)
     include(middleware_usb_device_ehci_LPC55S69_cm33_core0)
endif()
if(CONFIG_USE_middleware_usb_device_ip3511fs_LPC55S69_cm33_core0)
     include(middleware_usb_device_ip3511fs_LPC55S69_cm33_core0)
endif()
if(CONFIG_USE_middleware_usb_device_ip3511hs_LPC55S69_cm33_core0)
     include(middleware_usb_device_ip3511hs_LPC55S69_cm33_core0)
endif()
if(NOT (CONFIG_USE_middleware_usb_device_khci_LPC55S69_cm33_core0 OR CONFIG_USE_middleware_usb_device_ehci_LPC55S69_cm33_core0 OR CONFIG_USE_middleware_usb_device_ip3511fs_LPC55S69_cm33_core0 OR CONFIG_USE_middleware_usb_device_ip3511hs_LPC55S69_cm33_core0))
    message(WARNING "Since middleware_usb_device_khci_LPC55S69_cm33_core0/middleware_usb_device_ehci_LPC55S69_cm33_core0/middleware_usb_device_ip3511fs_LPC55S69_cm33_core0/middleware_usb_device_ip3511hs_LPC55S69_cm33_core0 is not included at first or config in config.cmake file, use middleware_usb_device_khci_LPC55S69_cm33_core0 by default.")
    include(middleware_usb_device_khci_LPC55S69_cm33_core0)
endif()

include(component_osa_LPC55S69_cm33_core0)
