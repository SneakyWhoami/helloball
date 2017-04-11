HelloBall
====

HelloBall is an experiment we made to study how to rewrite Balsamiq Mockups application using native technologies on IOS, Windows, Android, MacOS and Linux.

The path we choose has been to write 

1. a common Core Model, written in Javascript and running inside a JVM, containing data structures and algorithm
2. native technologies for the UI layer and external access to each platform service 
3. a double interface to communicate between javascript and native

Building HelloBall
=============

Building on MacOS/IOS/Windows
-----------------

Just open the corresponding project file

Building on Linux
-----------------

We lost the makefile but it shouldn't be difficult to write a new one

Building for Android
-----------------

Use IntelliJ Idea to open each project folder

Building for Web
-----------------

There is nothing to build, just open the ```helloball.html``` to see the web app working

Presentations
=============
 * [Hello Ball: un punto di partenza per applicazioni multi-piattaforma](http://2017.universaljsday.com)


License
=====
The code is published under the terms of the [Eclipse Public License, version 1.0](http://www.eclipse.org/legal/epl-v10.html).