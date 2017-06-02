
function getBrandSize() {
    var brandLabelSize;
    if (uiOrientation == Qt.PrimaryOrientation)
        brandLabelSize = 50;
    if (uiOrientation == Qt.PortraitOrientation)
        brandLabelSize = 50;
    if (uiOrientation == Qt.LandscapeOrientation)
        brandLabelSize = 100;
    if (uiOrientation == Qt.InvertedPortraitOrientation)
        brandLabelSize = 50;
    if (uiOrientation == Qt.InvertedLandscapeOrientation)
        brandLabelSize = 100;
    console.log("Brand Label Size:",brandLabelSize)
    setUiNameSize(brandLabelSize);
    return brandLabelSize;
}

function isAssignmentOdCurretnUser(currentAssignmentName, currentUserId) {
    return currentAssignmentName.StartsWith(currentUserId) === -1 ? false : true
}