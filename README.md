# moose

Testing
=======

Using the [Catch unit test framework](https://github.com/philsquared/Catch). There are two options for testing our plug-in code:

1. Include test code in the built shared library. Create an executable project which loads the library and runs tests.
2. Don't include test code in the library project. Create an executable project which contains all source code and tests, and which runs tests.

Went with second option; unsure how to implement first one. Thus there are two IntroJucer projects; Moose and MooseTest. The Moose.jucer file creates project for building the plug-in and does not include testing code. The MooseTest.jucer file creates project for building an executable which runs tests. Therefore, switching between testing and building the plug-in requires running the IntroJucer, as the file ~/JuceLibraryCode/AppConfig.h is significantly different when building a shared library vs. an executable.


Ubuntu Setup
============

First install all [dependencies](http://www.juce.com/forum/topic/list-juce-dependancies-under-ubuntu-linux):

<pre><code>
sudo apt-get -y install g++ libfreetype6-dev libx11-dev libxinerama-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev
</code></pre>

Ubuntu seems to [put freetype headers in an unexpected location](http://stackoverflow.com/questions/21213380/cannot-include-freetype-headers-on-linux). Therefore projects must include path /usr/include/freetype2.

The IntroJucer doesn't play nice with environment variables, otherwise we wouldn't have to hard-code header location of libraries. After checking out code, open the Moose.juce file and change the following in "Linux Makefile" target.

1. JuceLibraryCode/modules
2. VST

The IntroJucer only produces Code::Blocks files suitable for Windows. Here are steps which produce working Linux project files.

1. Add -fPIC compiler flag.
2. Define LINUX=1.
3. Explicitly include the VST3 directory in header search path.
4. After running IntroJucer, [remove all Windows libraries from *.cbp file](http://www.juce.com/forum/topic/codeblocks-missing-compiler-flag-and-libraries).
5. [Another useful link on the topic.](http://www.juce.com/forum/topic/juce-and-ides-under-linux)

ALSA creates problems for the Audio Plugin Host application. To get rid of the 256 input and ouput channels, [following these directions](http://www.juce.com/forum/topic/plugin-host-number-ins/outs).
