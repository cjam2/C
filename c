// Fetch the selected Product Group (Make sure it's correctly referenced)
def productGroup = binding.variables.get("Product_Group")

// Debugging: Print the selected value in Jenkins logs
println "Selected Product Group: " + productGroup

// Define malcode lists based on the selected product group
def malcodes = []
if (productGroup == "cod") {
    malcodes = ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"]
} else if (productGroup == "acc") {
    malcodes = ["CSps", "TSIhe"]
}

// If malcodes list is empty, return a default message
if (malcodes.isEmpty()) {
    return ["No Malcodes Available"]
}

// Debugging: Print the generated malcodes list in Jenkins logs
println "Generated Malcodes: " + malcodes

// Return the malcodes list (Jenkins will generate checkboxes automatically)
return malcodes