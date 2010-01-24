// FilePicker
const nsIFilePicker = Components.interfaces.nsIFilePicker;
const tbGnokiiFile = 'tbFile';

function Start()
{
    var tbFileName = document.getElementById( tbGnokiiFile );

    doConvertLocalfileToURIFile( tbFileName.value );
    // create file with this value
    // check if is a file

    //tbFileName.value = tbFileName.value + '1';
}

function onLoad()
{

}

function onUnLoad()
{

}

function doBrowse()
{
  // get instance of Textbox with Gnokii-File
  var tbFileName = document.getElementById( tbGnokiiFile );

  tbFileName.value = 'ReadFile';

  var oFilePicker = Components.classes["@mozilla.org/filepicker;1"]
                   .createInstance(nsIFilePicker);
  oFilePicker.appendFilters (nsIFilePicker.filterAll | nsIFilePicker.filterText );
  oFilePicker.init( window, "Select Gnokii-File", nsIFilePicker.modeOpen);

  var eReturnFP = oFilePicker.show();
  // check if return value is ok
  if (eReturnFP == nsIFilePicker.returnOK)
  {
    //read file 
    var oFile = oFilePicker.file;
    
    tbFileName.value = 'File selected';
    tbFileName.value = oFile.path;
  }
  else
  {
    tbFileName.value = 'Wrong ReturnValue';
  }
  
}

function doConvertLocalfileToURIFile( strLocalFile )
{
  alert(strLocalFile);

  var strUriFile = '';
  if( strLocalFile.IndexOf('file://')  >= 0)
  {
    alert('Path is already in URI format');
  }else
  {
    alert('Do Convert');
  }
}