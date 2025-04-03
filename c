def html = ""
def selectedMalcodes = MALCODES?.split(',') ?: []

selectedMalcodes.eachWithIndex { malcode, idx ->
    html += """<label for="input_${malcode}">Input for ${malcode}:</label>
               <input type="text" name="input_${malcode}" id="input_${malcode}" /><br><br>"""
}
return html