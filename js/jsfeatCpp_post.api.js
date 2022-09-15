Module["onRuntimeInitialized"] = function() {
    var event = new Event('jsfeatCpp-loaded');
    window.dispatchEvent(event);
}