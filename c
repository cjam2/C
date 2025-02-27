// Retrieve the selected Product Group (ensure the dropdown is named exactly "Product_Group")
def productGroup = binding.variables.get("Product_Group") ?: ""

// Debug output (visible in Jenkins logs)
println "DEBUG: Product_Group -> " + productGroup

// Define malcode lists for each product group
def malcodes = []
if (productGroup == "cod") {
    malcodes = ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"]
} else if (productGroup == "acc") {
    malcodes = ["CSps", "TSIhe"]
}

// Start building the HTML content for the parameter
def html = """
<html>
  <head>
    <style>
      body { font-family: Arial, sans-serif; margin: 0; padding: 0; }
      h3 { margin-bottom: 10px; }
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

if(malcodes.size() > 0) {
    malcodes.each { code ->
        html += """<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"""
    }
} else {
    html += "<p style='color: red;'>No Malcodes Available</p>"
}

html += """
    </div>
  </body>
</html>
"""

// Return the HTML which will be rendered in the parameter UI
return html