
$Version = "v1.0.5"
$DistPath = "$Home\Documents\Regatron\v1.0.5"

$Dipoles = @(
)

$Quadrupoles = @(
)

$Sextupoles = @(
    [pscustomobject]@{Port='127';Device='SDAP0'}
    [pscustomobject]@{Port='128';Device='SDB0'}
    [pscustomobject]@{Port='129';Device='SFAP0'}
    [pscustomobject]@{Port='130';Device='SFB0'}
    [pscustomobject]@{Port='131';Device='SDB1'}
    [pscustomobject]@{Port='132';Device='SDA12'}
    [pscustomobject]@{Port='133';Device='SDA3SFA1'}
    [pscustomobject]@{Port='134';Device='SDB2'}
    [pscustomobject]@{Port='135';Device='SFA2SDP1'}
    [pscustomobject]@{Port='136';Device='SDB3'}
    [pscustomobject]@{Port='137';Device='SDP23'}
    [pscustomobject]@{Port='138';Device='SFB1'}
    [pscustomobject]@{Port='139';Device='SFP12'}
    [pscustomobject]@{Port='140';Device='SFB2'}
    
)

function CreateShortcut {
    #param ( [string]$SourceExe, [string]$ArgumentsToSourceExe, [string]$DestinationPath )
    param ( [string]$DistPath, [string]$ArgumentsToSourceExe, [string]$DestinationPath, [string]$Desc )

    $WshShell = New-Object -comObject WScript.Shell
    $Shortcut = $WshShell.CreateShortcut($DestinationPath)
    $Shortcut.TargetPath = "$DistPath\cons_regatron_interface.exe"
    $Shortcut.Arguments = $ArgumentsToSourceExe
    $Shortcut.IconLocation = "$DistPath\cons_regatron_interface.ico"
    $Shortcut.WorkingDirectory = "$DistPath\"
    $Shortcut.Description = "Regatron DCLink - $Desc"
    $Shortcut.Save()
}

ForEach($d in $Dipoles){
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Dipoles\COM$($d.Port) $Version - $($d.Device).lnk" "Dipoles $Version"
}
ForEach($d in $Quadrupoles){
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Quadrupoles\COM$($d.Port) $Version - $($d.Device).lnk" "Quadrupoles $Version"
}
ForEach($d in $Sextupoles){
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Sextupoles\COM$($d.Port) $Version - $($d.Device).lnk" "Sextupoles $Version"
}
