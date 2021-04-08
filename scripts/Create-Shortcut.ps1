
$Version = "v1.0.5"
$DistPath = "$Home\Documents\Regatron\v1.0.5"

$Dipoles = @(
    [pscustomobject]@{Port='101';Device='D1 1A'}
    [pscustomobject]@{Port='102';Device='D1 1B'}
    [pscustomobject]@{Port='103';Device='D3 2A'}
    [pscustomobject]@{Port='104';Device='D3 2B'}
    [pscustomobject]@{Port='105';Device='D1 3A'}
    [pscustomobject]@{Port='106';Device='D1 3B'}
    [pscustomobject]@{Port='107';Device='D3 4A'}
    [pscustomobject]@{Port='108';Device='D3 4B'}
    [pscustomobject]@{Port='109';Device='D5 1A'}
    [pscustomobject]@{Port='110';Device='D5 1B'}
    [pscustomobject]@{Port='111';Device='D7 2A'}
    [pscustomobject]@{Port='112';Device='D7 2B'}
    [pscustomobject]@{Port='113';Device='D5 3A'}
    [pscustomobject]@{Port='114';Device='D5 3B'}
    [pscustomobject]@{Port='115';Device='D7 4A'}
    [pscustomobject]@{Port='116';Device='D7 4B'}
)

$Quadrupoles = @(
    [pscustomobject]@{Port='117';Device='QFAP'}
    [pscustomobject]@{Port='118';Device='QFB'}
    [pscustomobject]@{Port='119';Device='QDAP'}
    [pscustomobject]@{Port='120';Device='QDB'}
    [pscustomobject]@{Port='121';Device='Q13A'}
    [pscustomobject]@{Port='122';Device='Q13B'}
    [pscustomobject]@{Port='123';Device='Q13C'}
    [pscustomobject]@{Port='124';Device='Q24A'}
    [pscustomobject]@{Port='125';Device='Q24B'}
    [pscustomobject]@{Port='126';Device='Q24C'}
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
    New-Item -ItemType Directory -Force -Path "$Home\Desktop\Dipoles"
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Dipoles\COM$($d.Port) $Version - $($d.Device).lnk" "Dipoles $Version"
}
ForEach($d in $Quadrupoles){
    New-Item -ItemType Directory -Force -Path "$Home\Desktop\Quadrupoles"
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Quadrupoles\COM$($d.Port) $Version - $($d.Device).lnk" "Quadrupoles $Version"
}
ForEach($d in $Sextupoles){
    New-Item -ItemType Directory -Force -Path "$Home\Desktop\Sextupoles"
    CreateShortcut $DistPath $($d.Port) "$Home\Desktop\Sextupoles\COM$($d.Port) $Version - $($d.Device).lnk" "Sextupoles $Version"
}
