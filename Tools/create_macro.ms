try destroyDialog ::createMacroscript catch()
if NOT isKindOf createMacroscript RolloutClass do
	rollout createMacroscript "Create MacroScript" width:250 height:157
	(
		label lblCategory "Category: " pos:[8, 6]
		editText ebxCategory pos:[75, 6] fieldWidth:160
		label lblID "Script ID: " pos:[8, 28]
		editText ebxID pos:[75, 28] fieldWidth:160
		label lblName "Button Text: " pos:[8, 50]
		editText ebxName pos:[75, 50] fieldWidth:160
		label lblTooltip "Tooltip: " pos:[8, 72]
		editText ebxTooltip pos:[75, 72] fieldWidth:160
		label lblPath "Script Path: " pos:[8, 94]
		editText ebxPath pos:[75, 94] fieldWidth:140
		button btnGetPath "..." pos:[223,94] width:16 height:17
		progressBar pbFrame across:3 width:72 height:27 offset:[0, 7] enabled:false
		button btnEdit "Edit..." width:70 height:25 offset:[0, 8]
		button btnCancel "Cancel" width:70 height:25 offset:[0, 8]
		button btnExecute "EXECUTE" width:70 height:25 pos:(pbFrame.pos + [1, 1])

		local path
		local validChars = #{65..90, 97..122}
		local screenPos = if screenPos != undefined then screenPos

		fn tryGetPath &path =
			(path = getOpenFileName caption:"Select Script" \
				types:"Script files (*.ms,*.mse)|*.ms;*.mse|All Files|*.*|") != undefined

		fn cleanString str res:"" i:0 =
			while i < str.count do
				if validChars[bit.charAsInt (local char = str[i+=1])] then
					res += char
				else res
	
		fn formatMacro stream =
			format ("macroScript %\n\tcategory:\"%\"\n" + 
				(if ebxTooltip.text.count > 1 then "\ttooltip:\"%\"\n" else "%") +
				(if ebxName.text.count > 1 then "\tbuttonText:\"%\"\n" else "%") +
				"(\n\ton execute do fileIn \"%\"\n)\n") \
				ebxID.text ebxCategory.text ebxTooltip.text ebxName.text \
				(substituteString ebxPath.text "\\" "\\\\") to:stream
	
		on createMacroscript open do
			if screenPos != undefined do
				setDialogPos createMacroscript screenPos

		on btnGetPath pressed do
			if tryGetPath &path do
				ebxPath.text = path
	
		on btnEdit pressed do
			formatMacro (newScript())
	
		on btnExecute pressed do
		(
			if ebxID.text.count < 1 OR ebxCategory.text.count < 1 do
				return messageBox "First two items have to be filled in."
			if (ebxID.text = cleanString ebxID.text).count < 1 do
				return messageBox "Please correct the script ID text."
			if NOT pathConfig.isLegalPath ebxPath.text do
				return messageBox "Not a legal path."
			if NOT doesFileExist ebxPath.text do
				return messageBox "Selected file does not exist."

			local stream = stringStream ""
			formatMacro stream
			seek stream 0
			readExpr stream
		)
	
		on btnCancel pressed do
			destroyDialog createMacroscript
	
		on createMacroscript close do
			screenPos = getDialogPos createMacroscript
	)
createDialog createMacroscript
