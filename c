{confiforms:Form Name|name=EmailMapping}
{confiforms-field:recipientName|type=text}
{confiforms-field:recipientEmail|type=text}
{confiforms:Form Name}



{confiforms-table:EmailMapping}


{confiforms:Form Name|name=RequestForm}
{confiforms-field:recipientName|type=dropdown}
John:sr_john
Mary:br_mary
Team:tr_team
{confiforms-field}
{confiforms-field:summary|type=text}
{confiforms:Form Name}


{confiforms-ifttt:RequestForm}
{confiforms-ifttt-action:Send Email|to=[entry.recipientName.recipientEmail]|subject=CMR Received: [entry.summary]|body=Hello, a new form has been submitted by [entry.summary].}
{confiforms-ifttt}
