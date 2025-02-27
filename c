// Directly use the global variable Product_Group
def html = "<html><head><style>" +
           "body { font-family: Arial, sans-serif; }" +
           ".checkbox-grid { display: flex; flex-wrap: wrap; max-width: 500px; }" +
           ".checkbox-grid label { flex: 1 1 30%; margin: 5px; padding: 5px; border: 1px solid #ccc; border-radius: 4px; background-color: #f9f9f9; cursor: pointer; }" +
           ".checkbox-grid input { margin-right: 5px; }" +
           "</style></head><body>"

if (Product_Group == "COF") {
    def malcodes = ["CLK", "CLK HOST", "LSR", "CLKAI", "COS WS"]
    html += "<h3>Malcodes for COF</h3><div class='checkbox-grid'>"
    malcodes.each { code ->
       html += "<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"
    }
    html += "</div>"
} else if (Product_Group == "UCC") {
    def malcodes = ["VALUE1", "VALUE2", "VALUE3"]
    html += "<h3>Malcodes for UCC</h3><div class='checkbox-grid'>"
    malcodes.each { code ->
       html += "<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"
    }
    html += "</div>"
} else if (Product_Group == "BCR") {
    def malcodes = ["AMTM", "AMTT"]
    html += "<h3>Malcodes for BCR</h3><div class='checkbox-grid'>"
    malcodes.each { code ->
       html += "<label><input type='checkbox' name='malcode' value='${code}'> ${code}</label>"
    }
    html += "</div>"
} else {
    html += "<p style='color:red;'>No Malcodes Available for " + Product_Group + "</p>"
}

html += "</body></html>"
return html