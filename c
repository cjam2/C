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
      margin-bottom: 8px; /* Increased spacing */
    }
    .malcode-item input[type="checkbox"] {
      width: 20px;
      height: 20px;
      margin-right: 8px; /* Increased spacing */
      border: 1px solid #ccc; /* Add a border */
      border-radius: 3px; /* Slightly rounded corners */
    }
    .malcode-item label {
      font-size: 16px;
      color: #333; /* Darker text color */
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
  // Return the default checkboxes if Product_Group is not "COF"
  // You might need to adjust this depending on how Jenkins handles the default behavior
  return null; // Or return whatever Jenkins expects for default behavior
}
