// Use params instead of binding.variables
def productGroup = params.Product_Group ?: ""
println "DEBUG: Product_Group is: " + productGroup

// Define malcode lists based on the selected product group
def malcodesMap = [
    "cod": ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"],
    "acc": ["CSps", "TSIhe"]
]

// If no product group is selected, show a message
if (productGroup.trim() == "") {
    return "<p style='color:red;'>No Product Group Selected!</p>"
}

// If the product group isn't recognized, show an error
if (!malcodesMap.containsKey(productGroup)) {
    return "<p style='color:red;'>Unknown MALCODE: " + productGroup + "</p>"
}

// Get the malcodes for the selected product group
def malcodes = malcodesMap.get(productGroup)
println "DEBUG: Malcodes for " + productGroup + " -> " + malcodes

// Build HTML with styled checkboxes
def html = """
<html>
  <head>
    <style>
      body { font-family: Arial, sans-serif; }
      .checkbox-grid {
          display: flex;
          flex-wrap: wrap;
          max-width: 500px;
      }
      .checkbox-grid label {
          flex: 1 1 30%;
          margin: 5px;
          padding: 5px;
          border: 1px solid #ccc;
          border-radius: 4px;
          background-color: #f9f9f9;
          cursor: pointer;
      }
      .checkbox-grid input {
          margin-right: 5px;
      }
    </style>
  </head>
  <body>
    <h3>Available Malcodes for Product Group: ${productGroup.toUpperCase()}</h3>
    <div class="checkbox-grid">
"""

malcodes.each { code ->
    html += """<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"""
}

html += """
    </div>
  </body>
</html>
"""

return html