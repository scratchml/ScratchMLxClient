# ScratchMLxClient 

Version: BETA.0

authors: @ScratchML, so blame/tweet them if it sucks ballz

[ScratchML](www.scratchml.com)

[Github](https://github.com/scratchml)


This is an openFrameworks app and is release in source code only (binary releases coming soon!): 
## [openFrameworks](http://openframeworks.cc/) is a C++ toolkit for creative coding

##External addons:

###[ofxSoundStream] (https://github.com/jocabola/ofxSoundStream)
#####git clone git://github.com/jocabola/ofxSoundStream.git

###[ofxXwax] (http://github.com/scratchml/ofxXwax)
#####git clone git://github.com/scratchml/ofxXwax.git


##About
Most of this work is by @kcimc , Lee Meredith, and @theDANtheMAN.  In this release we can track as many Turntables as you have audio inputs.  It will write the raw SML performace tag to XML and also send OSC Messages to the defined host:port.  This is soo beta it hasn't been tested with actual turntables, but hopefully that will change.


##Setup


###Configure the settings.xml file:

<sml>
<record>
<!--
format can be: serato_2a, serato_2b, serato_cd, traktor_a,
traktor_b, mixvibes_v2, mixvibes_7inch
-->
<format>serato_2a</format>
<side>right</side>
<!-- changes the OSC message address -->
</record>
<osc>
<!-- change to X.X.X.X.255 where XXX is your IP address --> 
<host>localhost</host>
<port>12234</port>
<subdivide>8</subdivide> <!-- send every Nth message from the vinyl -->
<relative>0</relative> <!-- use relative instead of absolute position -->
<pitch>0</pitch> <!-- send pitch data -->
<degrees>0</degrees> <!-- send orientation in degrees -->
</osc>
<audio>
<!-- the samplerate/buffersize should match your audio settings -->
<sampleRate>44100</sampleRate>
<buffersize>16</buffersize>
<!-- Device Name -->
<interface></interface>
<!-- if Device name is not present it will default 
to the device id on a mac the audio-in is device 1-->
<device>1</device>
</audio>
<turntables>
<decks>1</decks>
<deck0>left</deck0>
<deck1>right</deck1>
<deck2>left_two</deck2>
<deck3>right_two</deck3>
</turntables>
<!-- serial is disabled -->
<serial>
<!-- the arduino should report a stream of bytes on this port was tty.usbmodemfd121-->
<port>/dev/tty.usbmodem411</port>
<!--
when the serial data passes this threshold in either direction,
a 0 or 1 is sent over OSC. in the future, this should be double-ended
so that it sends 0, .5, or 1.
-->
<threshold>240</threshold>
</serial>
</sml>


#SML File
Below is an exmaple of the SML file, `data/Session-X*.xml`, that will be generated.  

<sml>
<version>
<major>0</major>
<minor>6</minor>
</version>
<about>
<client>--ScratchML-- Version: BETA.0</client>
<authors>@ScratchML, so blame/tweet them if it sucks ballz</authors>
<url>www.scratchml.com</url>
<github>https://github.com/scratchml</github>
</about>
<audio>
<sample>
<filename>connect me to the GUI</filename>
<description>connect me to the GUI</description>
<deck>connect me to the GUI</deck>
<start>connect me to the GUI</start>
<stop>connect me ot the GUI</stop>
</sample>
</audio>
<performance>
<turntable>
<deck0>
<samplerate>375</samplerate>
<data></data>
</deck0>
<deck1>
<samplerate>375</samplerate>
<data></data>
</deck1>
</turntable>
</performance>
</sml>

if you've read this far stay tuned for ScratchMLxEmbedded and ScratchMLxRaspberryPi 

[You Ain't No DJ](http://www.youtube.com/watch?v=0tq8F_vDN_E)

[Represent the DirtyDirty](http://www.youtube.com/watch?v=3SMhxOgf7ys)