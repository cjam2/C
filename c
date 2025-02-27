#!/bin/bash

# Function to get filtered malcodes
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

  # Ensure proper output handling
  if [[ ${#final_arr[@]} -eq 0 ]]; then
    echo "No Malcode Upgrade Needed"
  else
    printf "%s\n" "${final_arr[@]}"  # Prints each malcode on a new line
  fi
}

# Define arrays
allowed_malcodes=("mal1" "mal2" "mal3")
got_malcode=("mal7" "mal2" "mal3")

# Capture filtered malcodes
filtered_malcodes=$(get_final_malcode allowed_malcodes got_malcode)

# Create HTML file
EMAIL_FILE="${WORKSPACE}/email_body.html"

echo "<html><head><style>
  body { font-family: Arial, sans-serif; }
  table { width: 100%; border-collapse: collapse; margin-top: 10px; }
  th, td { border: 1px solid black; padding: 10px; text-align: left; vertical-align: top; }
  th { background-color: #f2f2f2; font-weight: bold; }
</style></head><body>" > "$EMAIL_FILE"

echo "<h2>MALCODE Upgraded for ${month} Release in Credit Platform</h2>" >> "$EMAIL_FILE"
echo "<table>" >> "$EMAIL_FILE"
echo "<tr><th>Product Group</th><th>Malcodes</th></tr>" >> "$EMAIL_FILE"

# Add filtered malcodes to the table (Ensuring line breaks)
echo "<tr><td><b>Filtered Malcodes</b></td><td><pre>$filtered_malcodes</pre></td></tr>" >> "$EMAIL_FILE"

echo "</table></body></html>" >> "$EMAIL_FILE"













___________________









# Function to capture final malcodes correctly
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

  # Ensure output is properly captured
  if [[ ${#final_arr[@]} -eq 0 ]]; then
    echo "No Malcode Upgrade Needed"
  else
    printf "%s\n" "${final_arr[@]}"  # Each malcode on a new line
  fi
}

# Define arrays
allowed_malcodes=("mal1" "mal2" "mal3")
got_malcode=("mal7" "mal2" "mal3")

# Capture the filtered malcodes in a variable
filtered_malcodes=$(get_final_malcode allowed_malcodes got_malcode)

# Function to add a row to the table
add_table_row() {
    local title="$1"
    local values="$2"  # Capture values as a string

    echo "<tr><td><b>${title}</b></td><td>" >> "$EMAIL_FILE"

    # Ensure proper new-line formatting in HTML
    if [[ "$values" == "No Malcode Upgrade Needed" || -z "$values" ]]; then
        echo "$values" >> "$EMAIL_FILE"
    else
        echo "<pre>$values</pre>" >> "$EMAIL_FILE"  # Keeps line breaks intact
    fi

    echo "</td></tr>" >> "$EMAIL_FILE"
}

# Create HTML file
EMAIL_FILE="${WORKSPACE}/email_body.html"

echo "<html><head><style>
  body { font-family: Arial, sans-serif; }
  table { width: 100%; border-collapse: collapse; margin-top: 10px; }
  th, td { border: 1px solid black; padding: 10px; text-align: left; vertical-align: top; }
  th { background-color: #f2f2f2; font-weight: bold; }
</style></head><body>" > "$EMAIL_FILE"

echo "<h2>MALCODE Upgraded for ${month} Release in Credit Platform</h2>" >> "$EMAIL_FILE"
echo "<table>" >> "$EMAIL_FILE"
echo "<tr><th>Product Group</th><th>Malcodes</th></tr>" >> "$EMAIL_FILE"

# Add filtered malcodes to the email table
add_table_row "Filtered Malcodes" "$filtered_malcodes"

echo "</table></body></html>" >> "$EMAIL_FILE"