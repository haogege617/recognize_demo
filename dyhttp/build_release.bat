cmd /c C:\Users\admin\.gradle\wrapper\dists\gradle-4.10.1-all\455itskqi2qtf0v2sja68alqd\gradle-4.10.1\bin\gradle assembleRelease


copy build\intermediates\ndkBuild\release\obj\local\arm64-v8a\libcurl.a outputlibrary\arm64-v8a
copy build\intermediates\ndkBuild\release\obj\local\armeabi\libcurl.a outputlibrary\armeabi
copy build\intermediates\ndkBuild\release\obj\local\armeabi-v7a\libcurl.a outputlibrary\armeabi-v7a
copy build\intermediates\ndkBuild\release\obj\local\x86\libcurl.a outputlibrary\x86
copy build\intermediates\ndkBuild\release\obj\local\x86_64\libcurl.a outputlibrary\x86_64

pause
