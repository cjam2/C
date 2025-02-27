# Function to add a row to the table
add_table_row() {
    local title="$1"
    local values_str="$2"  # Accept values as a string
    local values=()  # Initialize empty array

    # Convert space-separated string into an array
    IFS=' ' read -r -a values <<< "$values_str"

    echo "<tr><td><b>${title}</b></td><td>" >> "$EMAIL_FILE"

    # Correct condition to check if the array is empty or just whitespace
    if [[ -z "$values_str" || ${#values[@]} -eq 0 ]]; then
        echo "No Malcode Upgrade Needed according to the Technical Tracker" >> "$EMAIL_FILE"
    else
        for word in "${values[@]}"; do
            echo "$word<br>" >> "$EMAIL_FILE"
        done
    fi

    echo "</td></tr>" >> "$EMAIL_FILE"
}