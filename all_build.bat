set sh_folder=%~dp0
pushd %sh_folder%
if exist release rmdir release /s/q
call build debug
call build
popd