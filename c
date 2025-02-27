// Get the selected product group
def pg = binding.variables.get("pg")

// Define malcode lists based on the selected product group
def malcodes = []
if (pg == "cod") {
    malcodes = ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"]
} else if (pg == "acc") {
    malcodes = ["CSps", "TSIhe"]
}

// Return an empty selection if no malcodes exist
if (malcodes.isEmpty()) {
    return "<p style='color:red;'><b>No Malcodes Available for this Product Group</b></p>"
}

// Generate HTML for a multi-column layout with checkboxes
def html = """
<style>
  .checkbox-grid {
    display: flex;
    flex-wrap: wrap;
    max-width: 500px;
  }
  .checkbox-grid label {
    flex: 1 1 30%;
    margin-bottom: 5px;
  }
  .checkbox-grid input {
    margin-right: 5px;
  }
</style>

<div class="checkbox-grid">
"""

// Add checkboxes dynamically based on the selected product group
malcodes.each { code ->
    html += """<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"""
}

html += "</div>"

return html