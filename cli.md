## This file only contains CLI commands of the project

### /help
serial input: /help
returns: "Check the full CLI reference at: https://github.com/dmach7/Light-Control-Engine — navigate to cli.md in the main tree"

### display
serial input: display [name | rgb(r g b) | #FFFFFF]
displays the color on the LED, does not save anything to the system
after displaying, system asks: "Want to define brightness? Y/N"
if Y: system asks "Brightness (0-255):" and applies
if N: keeps current brightness
brightness set this way is temporary and resets on next display command

examples:
display rgb(255 255 255)
display #FFFFFF
display green

### array
serial input: array [function]
available functions: add, delete, editcode, rename
colors persist in NVS/Preferences and survive resets

add: system asks for a name → then asks for a code (rgb or hex) → asks "Want to set brightness for this color? Y/N" → if Y asks "Brightness (0-255):" and saves alongside the color → if N saves with brightness 255 → returns "Color successfully attached"

delete: system displays full array in numerical order → asks for a number → returns "Color: name, Number # was deleted"

editcode: system displays full array in numerical order → asks for a number → asks for new code (rgb or hex) → returns "Color: name, Number # was changed to (### ### ###) or #xxxxxx"

rename: system displays full array in numerical order → asks for a number → asks for new name → returns "Color number # was changed to: name"

unknown function: returns "unknown array function, check https://github.com/dmach7/Light-Control-Engine cli.md"

### return

type esc to go back to home, type back to go back only one step

<details>
<summary>Flowchart view</summary>

```mermaid
flowchart TD
    START([Serial Input]) --> ROUTE{Command?}

    %% HELP
    ROUTE -->|/help| HELP[Print: Check the full CLI reference at:\ngithub.com/dmach7/Light-Control-Engine\nnavigate to cli.md in the main tree]
    HELP --> START

    %% DISPLAY
    ROUTE -->|display| DISP_PARSE[Parse argument]
    DISP_PARSE --> DISP_TYPE{Input type?}
    DISP_TYPE -->|name| DISP_LOOKUP{Found in array?}
    DISP_TYPE -->|rgb r g b| DISP_APPLY
    DISP_TYPE -->|#HEX| DISP_APPLY
    DISP_LOOKUP -->|Yes| DISP_APPLY[setPixelColor + show]
    DISP_LOOKUP -->|No| DISP_NOT[Print: Not recognised]
    DISP_NOT --> START
    DISP_APPLY --> DISP_ASK{Want to define\nbrightness? Y/N}
    DISP_ASK -->|Y| DISP_BRIGHT_IN[Ask: Brightness 0-255]
    DISP_BRIGHT_IN --> DISP_BRIGHT_APPLY[setBrightness + show\ntemporary — resets on next display]
    DISP_BRIGHT_APPLY --> START
    DISP_ASK -->|N| DISP_KEEP[Keep current brightness]
    DISP_KEEP --> START

    %% ARRAY
    ROUTE -->|array| ARR_FN{Function?}

    %% array add
    ARR_FN -->|add| ADD_NAME[Ask: Name]
    ADD_NAME --> ADD_CODE[Ask: Code\nrgb or hex]
    ADD_CODE --> ADD_BRIGHT{Want to set\nbrightness? Y/N}
    ADD_BRIGHT -->|Y| ADD_BRIGHT_IN[Ask: Brightness 0-255]
    ADD_BRIGHT_IN --> ADD_SAVE[Save name + code + brightness\nto NVS/Preferences]
    ADD_BRIGHT -->|N| ADD_SAVE_DEF[Save name + code\nbrightness = 255\nto NVS/Preferences]
    ADD_SAVE --> ADD_OK[Print: Color successfully attached]
    ADD_SAVE_DEF --> ADD_OK
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

    %% array unknown
    ARR_FN -->|unknown| ARR_ERR[Print: unknown array function\ncheck github.com/dmach7/Light-Control-Engine cli.md]
    ARR_ERR --> START

    %% NAVIGATION
    ROUTE -->|unknown command| UNKNOWN[Print: Not recognised]
    UNKNOWN --> START

    %% ESC / BACK
    ROUTE -->|esc| HOME([Return to home])
    ROUTE -->|back| PREV([Return one step back])
```

</details>
