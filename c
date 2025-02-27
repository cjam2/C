// Fetch the selected Product Group from Jenkins (handle null cases)
def productGroup = binding.variables.get("Product_Group") ?: "none"

// Debugging output (optional, check Jenkins logs)
println "Selected Product Group: " + productGroup

// Define malcode lists for each Product Group
def malcodesMap = [
    "cod": ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"],
    "acc": ["CSps", "TSIhe"]
]

// Get malcodes based on the selected Product Group
def malcodes = malcodesMap.get(productGroup, [])  // Default to empty list if no match

// If malcodes are empty, return a message instead
if (malcodes.isEmpty()) {
    return ["No Malcodes Available"]
}

// Debugging output (optional)
println "Generated Malcodes: " + malcodes

// Return malcodes (Jenkins will automatically create checkboxes)
return malcodes