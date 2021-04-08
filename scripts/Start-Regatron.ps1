"Dipoles", "Quadrupoles", "Sextupoles" | ForEach-Object {

    Set-Location C:\Users\fontes-sirius\Desktop\$_

    Get-ChildItem -Filter *.lnk | ForEach-Object {
        Start-Process -FilePath $_.FullName
    }

}