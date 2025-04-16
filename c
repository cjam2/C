#set ($labels = [])
#foreach ($choice in $form.getValue("myChoices"))
  #if ($choice.label)
    #set ($dummy = $labels.add($choice.label))
  #elseif ($choice.name)
    #set ($dummy = $labels.add($choice.name))
  #else
    #set ($dummy = $labels.add($choice))
  #end
#end
$labels.join(", ")
