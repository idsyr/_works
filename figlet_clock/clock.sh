while sleep .3; do
date=$(date +%H:%M:%S)
output=$(clear && echo "" && echo "" && figlet -f ANSI-Shadows.flf "  $date")
echo "$output" 
done
