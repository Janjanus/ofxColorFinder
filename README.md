# ofxColorFinder

ofxColorFinder is a openFrameworks addon that can give you the dominant color of any image. The following snippet shows how to use the addon:

```cpp
ofImage image;
image.load("0.jpg");

ofColor dominantColor = ofxColorFinder::getColor(image);
```

You can also customize the algorithm so it favors colors you specify. This can be done by providing a callback to the `ofxColorFinder::getColor()` method. The following code favors dark colors:

```cpp
ofImage image;
image.load("0.jpg");

ofColor dominantColor = ofxColorFinder::getColor(image, [](int r, int g, int b) {
	return 768-r-g-b+1; 
});
```

*Favor hue*
![Favor hue](/example/bin/data/screenshots/screenshot-1.png?raw=true "Favor hue")

*Favor bright colors*
![Favor bright colors](/example/bin/data/screenshots/screenshot-2.png?raw=true "Favor bright colors")

*Favor dark colors*
![Favor dark colors](/example/bin/data/screenshots/screenshot-3.png?raw=true "Favor dark colors")

## ColorFinder
This addon is a C++ port of the ColorFinder library written in Javascript by [pieroxy][1]. You can read more [documentation][2] at his website or see his [online demo][2].

[1]: https://github.com/pieroxy
[2]: http://pieroxy.net/blog/pages/color-finder/index.html
[3]: http://pieroxy.net/blog/pages/color-finder/demo.html