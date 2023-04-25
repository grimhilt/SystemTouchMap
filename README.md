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