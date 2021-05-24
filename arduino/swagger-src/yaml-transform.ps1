$srcTxt = (gc .\yaml-min.txt) -replace '(.{119})(.)', "`$1`n`$2"
$srcTxt = "const char * swaggerYAML = " + [regex]::Replace($srcTxt, "^(.*)$", "`"`$1`"", "Multiline")
$sp27 = " " * 27
[regex]::Replace($srcTxt, "^(`".*)$", "$sp27`$1", "Multiline") + ";" >.\yaml-arduino.txt
