#!/bin/bash

# Hardcoded IPs for different environments
declare -A ENV_IPS
ENV_IPS["env1"]="192.168.1.10 192.168.1.11 192.168.1.12"
ENV_IPS["env2"]="192.168.2.10 192.168.2.11 192.168.2.12"
ENV_IPS["env3"]="192.168.3.10 192.168.3.11 192.168.3.12"

# Hardcoded application names for a, b, c
declare -A APP_NAMES
APP_NAMES["a"]="appA"
APP_NAMES["b"]="appB"
APP_NAMES["c"]="appC"

# Get environment from Jenkins dropdown parameter
env_selected="$ENV_SELECTED"  # Replace ENV_SELECTED with the parameter from Jenkins

# Get the JAR URLs from the Jenkins parameters
jar_url_a="$JAR_URL_A"  # Replace JAR_URL_A with the actual parameter for app A
jar_url_b="$JAR_URL_B"  # Replace JAR_URL_B with the actual parameter for app B
jar_url_c="$JAR_URL_C"  # Replace JAR_URL_C with the actual parameter for app C

# Function to log into the server, replace the jar and restart the application
function update_and_restart() {
    local ip=$1
    local app_name=$2
    local jar_url=$3

    # Check if the JAR URL is empty
    if [[ -z "$jar_url" ]]; then
        echo "No JAR URL provided for $app_name on $ip. Skipping update."
        return
    fi

    echo "Logging into server $ip to update $app_name..."

    ssh -o StrictHostKeyChecking=no -T springboot@$ip << EOF
        sudo -S -p "Enter password: " su -c "wget $jar_url -O /opt/springboot/applications/$app_name/$app_name.jar"
        sudo -S -p "Enter password: " su -c "systemctl restart $app_name"
EOF
}

# Logic to execute based on the selected environment
if [[ -n "${ENV_IPS[$env_selected]}" ]]; then
    # Get the IPs for the selected environment
    ips=(${ENV_IPS[$env_selected]})

    # Update and restart applications a, b, and c on the respective servers if JAR URLs are not empty
    update_and_restart "${ips[0]}" "${APP_NAMES[a]}" "$jar_url_a"
    update_and_restart "${ips[1]}" "${APP_NAMES[b]}" "$jar_url_b"
    update_and_restart "${ips[2]}" "${APP_NAMES[c]}" "$jar_url_c"
else
    echo "Invalid environment selected!"
    exit 1
fi

echo "Application updates completed!"