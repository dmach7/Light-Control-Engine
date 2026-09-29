## This file only contains CLI commands of the project

### /help

serial input: /help
returns: "Check the full CLI reference at: <https://github.com/dmach7/Light-Control-Engine> — navigate to cli.md in the main tree"

### display

serial input: display [name | rgb(r g b) | #FFFFFF]
displays the color on the LED at full brightness, does not save anything to the system
there is no brightness question anymore, brightness was removed in 4.0 (want it dimmer? use lower rgb values)
blocked while a fade is running, returns: "led busy: a fade is running, type stop to cancel it"

examples:
display rgb(255 255 255)
display #FFFFFF
display green
display rgb(60 0 0)

### array

serial input: array [function]
available functions: add, delete, editcode, rename, delete all
colors persist in NVS/Preferences and survive resets

add: system asks for a name → then asks for a code (rgb or hex) → returns "Color successfully attached"

delete: system displays full array in numerical order → asks for a number → returns "Color: name, Number # was deleted"

editcode: system displays full array in numerical order → asks for a number → asks for new code (rgb or hex) → returns "Color: name, Number # was changed to (### ### ###)"

rename: system displays full array in numerical order → asks for a number → asks for new name → returns "Color number # was changed to: name"

delete all: system asks "Are you sure? Y/N" → if Y wipes ALL memory (custom colors and saved fades) and goes back to factory → returns "Memory wiped, back to factory"
also works as: delete all
blocked while a fade is running

unknown function: returns "unknown array function, check <https://github.com/dmach7/Light-Control-Engine> cli.md"

### fade

serial input: fade [name | rgb(r g b) | #hex] [fade in time]
creates a fade with a normal (linear) curve
the line is split at the LAST space: the last word is the fade in time, everything before it is the color (so rgb(255 0 128) works)
fade in always starts from black (off) and goes up to the color
times need a suffix: 1.5s or 1500ms, no suffix returns an error
max time is 3600s, fade in and fade out can't be 0, hold can
can't fade to black (off), there is nothing to fade to
blocked while a fade is running

after the command, system asks: "Hold time (how long it stays lit, e.g. 2s):"
then asks: "Add fade out? Y/N"
if Y: system asks "Fade out time:" → returns "fade successfully created, saved by number #"
if N: returns "fade successfully created as number #" (after the hold the LED turns off instantly, no fade out)
then asks: "Execute now? Y/N"
if Y: returns "Running fade number #" and the fade runs in the background, the CLI keeps working
if N: goes back to the prompt, the fade stays saved
the fade prints nothing when it ends by itself

max 30 saved fades, when full returns "fade error: storage full"
fades persist in NVS/Preferences and survive resets

examples:
fade red 1.5s
fade #FF5500 800ms
fade rgb(255 0 128) 2s
fade Goose Turd Green 1s

errors:
fade error: missing color and time
fade error: missing fade in time
fade error: color not recognised
fade error: invalid time, use a suffix like 1.5s or 1500ms
fade error: time too long, max is 3600s
fade error: time can't be 0
fade error: can't fade to black, pick another color
fade error: storage full

### fadeg

serial input: fadeg [name | rgb(r g b) | #hex] [fade in time]
same as fade (same prompts, same rules, same saving) but with gamma correction (exponent 2.2)
linear fades look like they jump at the start and sit at full color for too long, gamma looks smoother to the eye
the curve is saved with the fade, fades list shows it

example:
fadeg #00FF88 2s

### fades

serial input: fades [function]
available functions: list, run, delete, edit
fades are saved by number, numbers start at 0 and shift down after a delete (same as array)
run, delete and edit accept the number inline or ask for it

list: system displays all saved fades in numerical order → each line shows number, color, curve (linear or gamma), fade in, hold and fade out (none if there isn't one)
no fades saved returns "fades error: no fades saved yet"

run: fades run [number] → if no number, system displays the list and asks "Number to run:" → returns "Running fade number #" and the fade runs in the background
blocked while a fade is running

delete: fades delete [number] → if no number, system displays the list and asks "Number to delete:" → returns "Fade number # was deleted"
deleting the fade that is running does not stop it, only stop does

edit: fades edit [number] → if no number, system displays the list and asks "Number to edit:" → asks "Edit what? (color, in, hold, out):"
color: asks "New color (name, rgb(r g b) or #hex):" → returns "Fade number #: color was changed to (### ### ###)"
in: asks "New fade in time:" → returns "Fade number #: fade in was changed to time"
hold: asks "New hold time:" → returns "Fade number #: hold was changed to time"
out: asks "New fade out time (or none to remove it):" → returns "Fade number #: fade out was changed to time" or "Fade number #: fade out was removed"
the curve (fade or fadeg) can't be edited, it comes from the command that created the fade

unknown function: returns "unknown fades function, check <https://github.com/dmach7/Light-Control-Engine> cli.md"

examples:
fades list
fades run 2
fades delete 0
fades edit 1

### stop

serial input: stop
cancels the running fade: the LED turns off immediately and the fade stays saved
returns "Fade stopped", or "stop: nothing is running" if there is no fade running
while a fade is running ONLY stop cancels it, esc and back don't
while a fade is running these are blocked: display, fade, fadeg, fades run, delete all
everything else keeps working (array add, delete, editcode, rename, fades list, delete, edit)
blocked commands return "led busy: a fade is running, type stop to cancel it"

### return

type esc to go back to home, type back to go back only one step
back and esc are reserved, they can't be used as color names
they work in every prompt (hold time, Add fade out?, Execute now?, edit steps, array steps...)

<details>
<summary><b>Flowchart view</b></summary>

```mermaid
flowchart TD
    START([Serial Input]) --> ROUTE{Command?}

    %% HELP
    ROUTE -->|/help| HELP[Print: Check the full CLI reference at:\ngithub.com/dmach7/Light-Control-Engine\nnavigate to cli.md in the main tree]
    HELP --> START

    %% DISPLAY
    ROUTE -->|display| DISP_BUSY{Fade running?}
    DISP_BUSY -->|Yes| BUSY[Print: led busy\ntype stop to cancel it]
    BUSY --> START
    DISP_BUSY -->|No| DISP_TYPE{Input type?}
    DISP_TYPE -->|name| DISP_LOOKUP{Found in array?}
    DISP_TYPE -->|rgb r g b| DISP_APPLY
    DISP_TYPE -->|#HEX| DISP_APPLY
    DISP_LOOKUP -->|Yes| DISP_APPLY[setPixelColor + show\nfull brightness]
    DISP_LOOKUP -->|No| DISP_NOT[Print: display error\ncolor not recognised]
    DISP_NOT --> START
    DISP_APPLY --> START

    %% ARRAY
    ROUTE -->|array| ARR_FN{Function?}

    %% array add
    ARR_FN -->|add| ADD_NAME[Ask: Name]
    ADD_NAME --> ADD_CODE[Ask: Code\nrgb or hex]
    ADD_CODE --> ADD_SAVE[Save name + code\nto NVS/Preferences]
    ADD_SAVE --> ADD_OK[Print: Color successfully attached]
    ADD_OK --> START

    %% array delete
    ARR_FN -->|delete| DEL_LIST[Display full array\nin numerical order]
    DEL_LIST --> DEL_ASK[Ask: Number]
    DEL_ASK --> DEL_DO[Remove from NVS/Preferences]
    DEL_DO --> DEL_OK[Print: Color: name, Number # was deleted]
    DEL_OK --> START

    %% array editcode
    ARR_FN -->|editcode| EDIT_LIST[Display full array\nin numerical order]
    EDIT_LIST --> EDIT_ASK[Ask: Number]
    EDIT_ASK --> EDIT_CODE[Ask: New code\nrgb or hex]
    EDIT_CODE --> EDIT_SAVE[Update NVS/Preferences]
    EDIT_SAVE --> EDIT_OK[Print: Color: name, Number # was changed to\nnew code]
    EDIT_OK --> START

    %% array rename
    ARR_FN -->|rename| REN_LIST[Display full array\nin numerical order]
    REN_LIST --> REN_ASK[Ask: Number]
    REN_ASK --> REN_NAME[Ask: New name]
    REN_NAME --> REN_SAVE[Update NVS/Preferences]
    REN_SAVE --> REN_OK[Print: Color number # was changed to: name]
    REN_OK --> START

    %% array delete all
    ARR_FN -->|delete all| WIPE_BUSY{Fade running?}
    WIPE_BUSY -->|Yes| BUSY
    WIPE_BUSY -->|No| WIPE_ASK{Are you sure? Y/N}
    WIPE_ASK -->|Y| WIPE_DO[Wipe colors + fades\nfactory state]
    WIPE_DO --> WIPE_OK[Print: Memory wiped, back to factory]
    WIPE_OK --> START
    WIPE_ASK -->|N| WIPE_NO[Print: Cancelled]
    WIPE_NO --> START

    %% array unknown
    ARR_FN -->|unknown| ARR_ERR[Print: unknown array function\ncheck github.com/dmach7/Light-Control-Engine cli.md]
    ARR_ERR --> START

    %% FADE / FADEG
    ROUTE -->|fade / fadeg| FADE_BUSY{Fade running?}
    FADE_BUSY -->|Yes| BUSY
    FADE_BUSY -->|No| FADE_PARSE[Split at last space\nlast word = fade in time\nrest = color]
    FADE_PARSE --> FADE_CHECK{Color and time valid?\nstorage not full?}
    FADE_CHECK -->|No| FADE_ERR[Print: fade error]
    FADE_ERR --> START
    FADE_CHECK -->|Yes| FADE_HOLD[Ask: Hold time]
    FADE_HOLD --> FADE_OUT_Q{Add fade out? Y/N}
    FADE_OUT_Q -->|Y| FADE_OUT_T[Ask: Fade out time]
    FADE_OUT_T --> FADE_SAVE_OUT[Save fade to NVS/Preferences]
    FADE_SAVE_OUT --> FADE_OK_OUT[Print: fade successfully created,\nsaved by number #]
    FADE_OUT_Q -->|N| FADE_SAVE[Save fade to NVS/Preferences]
    FADE_SAVE --> FADE_OK[Print: fade successfully created\nas number #]
    FADE_OK_OUT --> FADE_EXEC{Execute now? Y/N}
    FADE_OK --> FADE_EXEC
    FADE_EXEC -->|N| START
    FADE_EXEC -->|Y| RUN

    %% FADES
    ROUTE -->|fades| FS_FN{Function?}
    FS_FN -->|list| FS_LIST[Display all fades\nin numerical order]
    FS_LIST --> START
    FS_FN -->|run| FS_RUN_BUSY{Fade running?}
    FS_RUN_BUSY -->|Yes| BUSY
    FS_RUN_BUSY -->|No| FS_RUN_NUM[Number inline\nor list + ask number]
    FS_RUN_NUM --> RUN
    FS_FN -->|delete| FS_DEL_NUM[Number inline\nor list + ask number]
    FS_DEL_NUM --> FS_DEL_OK[Remove from NVS/Preferences\nPrint: Fade number # was deleted]
    FS_DEL_OK --> START
    FS_FN -->|edit| FS_EDIT_NUM[Number inline\nor list + ask number]
    FS_EDIT_NUM --> FS_EDIT_WHAT[Ask: Edit what?\ncolor / in / hold / out]
    FS_EDIT_WHAT --> FS_EDIT_VAL[Ask: new value]
    FS_EDIT_VAL --> FS_EDIT_OK[Update NVS/Preferences\nPrint: Fade number #: ... was changed]
    FS_EDIT_OK --> START
    FS_FN -->|unknown| FS_ERR[Print: unknown fades function\ncheck github.com/dmach7/Light-Control-Engine cli.md]
    FS_ERR --> START

    %% RUN (background)
    RUN([Print: Running fade number #\nfade runs in the background]) --> START

    %% STOP
    ROUTE -->|stop| STOP_Q{Fade running?}
    STOP_Q -->|Yes| STOP_DO[LED off immediately\nfade stays saved\nPrint: Fade stopped]
    STOP_Q -->|No| STOP_NO[Print: stop: nothing is running]
    STOP_DO --> START
    STOP_NO --> START

    %% NAVIGATION
    ROUTE -->|unknown command| UNKNOWN[Print: Not recognised]
    UNKNOWN --> START

    %% ESC / BACK
    ROUTE -->|esc| HOME([Return to home])
    ROUTE -->|back| PREV([Return one step back])
```

</details>
