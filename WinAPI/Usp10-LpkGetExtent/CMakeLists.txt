
add_executable(scriptstring main.c strings.rc)
set_module_type(scriptstring win32gui UNICODE)
add_importlibs(scriptstring usp10 gdi32 user32 msvcrt kernel32 ntdll)
add_rostests_file(TARGET scriptstring SUBDIR suppl)
