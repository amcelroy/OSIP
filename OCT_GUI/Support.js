//Check if a string is an Int or not
function checkTextIsInt(text){
    var check = parseInt(text);
    if(Number.isNaN(check)) {
        continueButton.enabled = false;
    }else{
        continueButton.enabled = true;
    }
}

//CHeck if a string is a Float or not
function checkTextIsFloat(text){
    var check = parseFloat(text);
    if(Number.isNaN(check)) {
        continueButton.enabled = false;
    }else{
        continueButton.enabled = true;
    }
}
