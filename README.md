# SystemTouchMap

A command line tool allowing you to log touchpad clicks on your computer and to create a color map of the clicks positions.

Behavior:
* A touch without moving on the touchpad counts as a click
* A left of right click on the touchpad (press not just touch) counts as a click
* Double click or touch counts as two clicks
* Click, move, unclick counts as a click
* Touch, click, untouch, counts as a click
* Scrolling doesn't count as a click
* Two fingers touch doesn't count as a click

## Context

I belong to the people that put their taskbar on top. And a lot of people ask me why, arguing that if it is on the bottom by default it must be better and they feel it is. For a long time, I responded without proof that it was more efficient as most of the clicks are done on the upper zone of your screen, effectively reducing the cursor's travel to the task bar.
I finally had the time to find the proof, I logged all my clicks for a week, accumulating more than 17000 clicks on my laptop and here are the results obtained with ``./main -a -f logs -c 200 -r 200 -s 200200.png`` showing a visual representation of my clicks in 200 columns and 200 rows: 
![200x200 map of clicks](https://github.com/grimhilt/medias/blob/master/SystemTouchMap/200200.png)

By separating my clicks in two rows (``./main -a -f logs -c 1 -r 2``) I can see that with my laptop usage (which was mostly code and browsing) most of my clicks are on the upper part of the screen (9052 on the upper part against 8194 on the bottom part). So it would indeed be more efficient to add the task bar on top. I need to precise that I am currently running on a linux with polybar so there is not a lot of clicks on the task bar even if we can notice some clicks to switch desktop. Finally I would say that now I prefer having my task bar on top by habits and because I find it easier to look in front of me to see the time or other informations than looking down.

## Usage

```console
Usage: main [OPTION...] COMMAND

  -a, --analyse              Analyse the data and show it
  -c, --column=NB_COL        The number of column the image will be divised in
  -f, --file=LOG_FILE        File where the clicks are logged (Default:
                             logs.txt)
  -h, --height=HEIGHT_PX     The height of the image in pixel
  -l, --log                  Log all clicks and save them to a file
  -r, --row=NB_ROW           The number of row the image will be divised in
  -s, --save=STATS_IMAGE     Save the data into a PNG file
  -w, --width=WIDTH_PX       The width of the image in pixel
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to https://github.com/grimhilt/SystemTouchMap/issues.
```

Use ``./main --log --file logs.txt`` to first logs every click and then when you have recover enough click you can generate a map with ``./main --analyse --file logs.txt``.

## Installation
It is quite the usual it use libinput and you can build the project with make. However as it works with libinput it will work only with linux and with xorg (used to find the touchpad event).
