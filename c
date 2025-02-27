// Grab the selected product group
def productGroup = binding.variables.get("ProductGroup")

// Debugging: Print to Jenkins logs
println "DEBUG: Selected ProductGroup -> " + productGroup

// Define possible malcodes per product group
def malcodesMap = [
    "cod": ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"],
    "acc": ["CSps", "TSIhe"]
]

// If no productGroup is selected yet, or it's null, just return an empty list
if (productGroup == null || productGroup.trim() == "") {
    return []
}

// If productGroup not in the map, show a warning
if (!malcodesMap.containsKey(productGroup)) {
    return ["Unknown MALCODE: " + productGroup]
}

// Otherwise, return the correct malcodes for this product group
def malcodes = malcodesMap[productGroup]
println "DEBUG: Malcodes -> " + malcodes
return malcodes