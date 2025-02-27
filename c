def productGroup = Product_Group // Assuming Product_Group is a parameter

if (productGroup == "COF") {
  def malcodes = ["CSL", "BRF", "CFP", "SAPI", "PLAPR", "PPA", "SOLIN", "PTTD", "BLDRA", "TINS", "TSI"] // Replace with your actual list

  def html = """
  <style>
    .malcode-list {
      list-style: none;
      padding: 0;
    }
    .malcode-item {
      display: flex;
      align-items: center;
      margin-bottom: 5px;
    }
    .malcode-item input[type="checkbox"] {
      width: 20px;
      height: 20px;
      margin-right: 5px;
    }
    .malcode-item label {
      font-size: 16px;
    }
    .malcode-item input[type="checkbox"]:checked + label {
      font-weight: bold;
    }
  </style>
  <ul class="malcode-list">
  """

  malcodes.each { malcode ->
    html += """
    <li class="malcode-item">
      <input type="checkbox" id="${malcode}" name="malcode" value="${malcode}">
      <label for="${malcode}">${malcode}</label>
    </li>
    """
  }

  html += "</ul>"
  return html
} else {
  return "" // Return an empty string if Product_Group is not "COF"
}
