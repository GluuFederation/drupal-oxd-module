(function($){
    //console.log("here");
    //alert("here"); // value
    
		if ($("#edit-enrollment-3").is(":checked")) 
		{
			$("#edit-user-type").attr("disabled","disabled");
		}
		
    $("input[name=enrollment]").click(function(){
		var enrollement=$('input[name=enrollment]:checked', '#default-form').val();
		if (enrollement=='3')
		{
			$("#edit-user-type").attr("disabled","disabled");
		}
		if(enrollement=='1')
		{
			$("#edit-user-type").removeAttr("disabled","disabled");
		}
	});
})(jQuery);
