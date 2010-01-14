function startGnokiiReader()
{
    window.openDialog("chrome://GnokiiReader/content/MainWindow/GnokiiReaderMain.xul",
                      "chrome-browser", "resizable,dialog=no,status",
                      {url: "chrome://"});
}
