#!/bin/bash

allowed_malcodes=("mal1" "mal2" "mal3")
got_malcode=("mal7" "mal2" "mal3")

get_final_malcode() {
  local -n allowed_arr=$1
  local -n got_arr=$2
  local final_arr=()

  for gm in "${got_arr[@]}"; do
    for am in "${allowed_arr[@]}"; do
      if [[ "$gm" == "$am" ]]; then
        final_arr+=("$gm")
        break
      fi
    done
  done

  # Echo space-separated result
  echo "${final_arr[@]}"
}

# Call the function
final_malcode=$(get_final_malcode allowed_malcodes got_malcode)

echo "Final malcode array: $final_malcode"