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
      margin-bottom: 8px;
    }
    .malcode-item input[type="checkbox"] {
      width: 20px;
      height: 20px;
      margin-right: 8px;
      border: 1px solid #ccc;
      border-radius: 3px;
    }
    .malcode-item label {
      font-size: 16px;
      color: #333;
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
  // Return the default checkboxes (without styling)
  return """
    <input type="checkbox" name="malcode" value="CSL"> CSL<br>
    <input type="checkbox" name="malcode" value="BRF"> BRF<br>
    <input type="checkbox" name="malcode" value="CFP"> CFP<br>
    <input type="checkbox" name="malcode" value="SAPI"> SAPI<br>
    <input type="checkbox" name="malcode" value="PLAPR"> PLAPR<br>
    <input type="checkbox" name="malcode" value="PPA"> PPA<br>
    <input type="checkbox" name="malcode" value="SOLIN"> SOLIN<br>
    <input type="checkbox" name="malcode" value="PTTD"> PTTD<br>
    <input type="checkbox" name="malcode" value="BLDRA"> BLDRA<br>
    <input type="checkbox" name="malcode" value="TINS"> TINS<br>
    <input type="checkbox" name="malcode" value="TSI"> TSI<br>
  """
}
