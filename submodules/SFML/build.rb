
# Author: Pierce Brooks

require 'rubygems'
require 'fileutils'

if (Dir.exists?("build"))
	FileUtils.remove_entry("build")
end

Dir.mkdir("build")

cwd = Dir.getwd
Dir.chdir("build")

command = ["cmake", "-G", ARGV[0], "-DCMAKE_SYSTEM_NAME=Android", "-DCMAKE_ANDROID_NDK="+ARGV[1], "-DCMAKE_ANDROID_ARCH_ABI=armeabi-v7a", "-DCMAKE_ANDROID_STL_TYPE=c++_shared", "-DCMAKE_BUILD_TYPE=Debug", "-DCMAKE_ANDROID_NDK_TOOLCHAIN_VERSION=clang", "./../SFML"]
print(command.join(" ").to_s+"\n")
if not (system(*command))
	exit
end

command = ["cmake", "--build", "."]
print(command.join(" ").to_s+"\n")
if not (system(*command))
	exit
end

Dir.chdir(cwd)
