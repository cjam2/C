if (Product_Group == "cod") {
    return """
      <html>
        <body>
          <h3>COD Malcodes</h3>
          <div style='border:1px solid #ccc; padding:10px;'>
            <label><input type='checkbox' name='malcode' value='CSL'> CSL</label><br>
            <label><input type='checkbox' name='malcode' value='BRF'> BRF</label><br>
            <label><input type='checkbox' name='malcode' value='CFP'> CFP</label><br>
          </div>
        </body>
      </html>
    """
} else {
    return "<p style='color:red;'>No Malcodes Available</p>"
}