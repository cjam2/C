// Get the selected Product Group from Jenkins
def productGroup = binding.variables.get("Product_Group")

// Debugging: Print the selected value in Jenkins logs
println "DEBUG: Received Product Group -> " + productGroup

// Define malcode lists based on the selected product group
def malcodesMap = [
    "cod": ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"],
    "acc": ["CSps", "TSIhe"]
]

// Ensure the product group is recognized
if (!malcodesMap.containsKey(productGroup)) {
    return ["⚠️ Unknown MALCODE: " + productGroup]
}

// Retrieve malcodes for the selected product group
def malcodes = malcodesMap.get(productGroup)

// Debugging: Print the generated malcodes in Jenkins logs
println "DEBUG: Generated Malcodes -> " + malcodes

// Return the malcodes (Jenkins auto-generates checkboxes)
return malcodes