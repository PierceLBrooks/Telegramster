
# Author: Pierce Brooks

require 'rubygems'
require 'fileutils'

api = "21"
arch = "arm"
stl = "c++_static"
abi = "armeabi-v7a"
generator = ARGV[0]
ndk = ARGV[1]
sdk = ARGV[2]
build = "build"

if (Dir.exists?(build))
	FileUtils.remove_entry(build)
end

Dir.mkdir(build)

cwd = Dir.getwd
standalone = File.join(cwd, "standalone")

command = ["python", File.join(ndk, "build", "tools", "make_standalone_toolchain.py"), "--arch", arch, "--api", api, "--install-dir", standalone]
print(command.join(" ").to_s+"\n")

Dir.chdir(build)

cmake = File.join(sdk, "cmake")
entries = Dir.entries(cmake)
cmake = File.join(cmake, entries[entries.length-1])
cmake = File.join(cmake, "bin")
ninja = File.join(cmake, "ninja")
cmake = File.join(cmake, "cmake")

command = ["cmake", "-G", generator, "-DCMAKE_SYSTEM_NAME=Android", "-DCMAKE_SYSTEM_VERSION="+api, "-DCMAKE_ANDROID_NDK="+ndk, "-DCMAKE_ANDROID_STANDALONE_TOOLCHAIN="+standalone, "-DCMAKE_ANDROID_ARCH_ABI="+abi, "-DCMAKE_ANDROID_STL_TYPE="+stl, "-DCMAKE_BUILD_TYPE=Debug", "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang", "-DANDROID_STL="+stl, "-DANDROID_PLATFORM=android-"+api, "-DANDROID_NATIVE_API_LEVEL="+api, "-DANDROID_ABI="+abi, "-DANDROID_TOOLCHAIN=clang", "./../SFML"]
print(command.join(" ").to_s+"\n")
if not (system(*command))
	exit
end

command = [cmake, "--build", "."]
print(command.join(" ").to_s+"\n")
if not (system(*command))
	exit
end

Dir.chdir(cwd)
