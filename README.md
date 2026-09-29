# Light Control Engine

A lightweight RGB lighting control engine for the ESP32-S3.

## Version 2.0

In the second version i brought some good updates!

Now you can choose a color WITHOUT having to type the rgb code.

Basically i added an array with maped colors, example red

```
NamedColor colors[] = {
  {"red",     255, 0,   0  },
```

i added this with a bunch of other colors so you don't really need to look like an neandertal searching for RGB codes! yayy

> [!TIP]
> You can add more colors as you want, so if you want to add like turkish-blue, just add it to the array following the pattern.

also added hex code support

```
#FF5500
#00FF88
```

input priority: the prompt follows a priority of commands:

```
named color → hex → rgb values → "Not recognised"
```
And also to avoid dum ahh user frustration i added a script that every system should have

It is the:
```toLowerStr```
this function allows almost any type of input, so u don't especifically need to write "Red" for example, u can type "red" or "ReD" or "RED", the function was pretty hard to write but it makes the system much better

Here is the code for the function

```
void toLowerStr(char* str) {
  for (int i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') str[i] += 32;  //this block was a bitch to write, why so complicated, don't ask me how ts works
  }
```

Ignore angry dev comment

This is it for now, thanks for suporting(no one is supporting) but thanks anyway



