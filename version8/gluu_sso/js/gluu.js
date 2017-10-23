(function ($) {
    
    if ($("#edit-enrollment-3").is(":checked"))
    {
        $("#edit-user-type").attr("disabled", "disabled");
    }

    $("input[name=enrollment]").click(function () {
        var enrollement = $('input[name=enrollment]:checked', '#default-form').val();
        if (enrollement == '3')
        {
            $("#edit-user-type").attr("disabled", "disabled");
        }
        if (enrollement == '1')
        {
            $("#edit-user-type").removeAttr("disabled", "disabled");
        }
    });
    if($('input[name=connection_type]:checked').val() == 2){
        $('#edit-oxd-web-host').show();
        $('label[for="edit-oxd-web-host"]').show();
        $('#edit-oxd-port').hide();
        $('label[for="edit-oxd-port"]').hide();
    }else{
        $('#edit-oxd-web-host').hide();
        $('label[for="edit-oxd-web-host"]').hide();
        $('#edit-oxd-port').show();
        $('label[for="edit-oxd-port"]').show();
    }
    $('input[name=connection_type]').change(function(){
        if(this.value == 1){
            $('#edit-oxd-web-host').hide();
            $('label[for="edit-oxd-web-host"]').hide();
            $('#edit-oxd-port').show();
            $('label[for="edit-oxd-port"]').show();
        }else{
            $('#edit-oxd-web-host').show();
            $('label[for="edit-oxd-web-host"]').show();
            $('#edit-oxd-port').hide();
            $('label[for="edit-oxd-port"]').hide();
        }
    });
    if($("input[name=enrollment]:checked").val() != 2){
        $("#edit-enrollment div:nth-child(2)").append(
            "<div class=\"js-form-item form-item js-form-type-textfield form-type-textfield js-form-item-customurl form-item-customurl\">\n\
                <input name=\"gluu_new_role[]\" disabled placeholder=\"Input role name\" class=\"port form-text perm-btn\" style=\"display: inline; width: 200px !important; \" value=\"\" type=\"text\">\n\
                <button type=\"button\" class=\"btn btn-xs perm-btn addPerm\" disabled=\"true\"><span class=\"glyphicon glyphicon-plus\"></span></button>\n\
            </div>"
        );
    } else {
        var gluu_roles = $("input[name=enrollment]:checked").attr('data-gluu-roles');
        var gluu_roles_array = gluu_roles.split(' ');
        for (var i = 0; i < gluu_roles_array.length; i++ ) {
            if(i == 0){
                $("#edit-enrollment div:nth-child(2)").append(
                    "<div class=\"js-form-item form-item js-form-type-textfield form-type-textfield js-form-item-customurl form-item-customurl\">\n\
                        <input name=\"gluu_new_role[]\" placeholder=\"Input role name\" class=\"port form-text perm-btn\" style=\"display: inline; width: 200px !important; \" value=\""+gluu_roles_array[i]+"\" type=\"text\">\n\
                        <button type=\"button\" class=\"btn btn-xs perm-btn addPerm\"><span class=\"glyphicon glyphicon-plus\"></span></button>\n\
                    </div>"
                );
            } else {
                $("#edit-enrollment div:nth-child(2)").append(
                    "<div class=\"js-form-item form-item js-form-type-textfield form-type-textfield js-form-item-customurl form-item-customurl\">\n\
                        <input name=\"gluu_new_role[]\" placeholder=\"Input role name\" class=\"port form-text perm-btn\" style=\"display: inline; width: 200px !important; \" value=\""+gluu_roles_array[i]+"\" type=\"text\">\n\
                        <button type=\"button\" class=\"btn btn-xs perm-btn addPerm\"><span class=\"glyphicon glyphicon-plus\"></span></button>\n\
                        <button type=\"button\" class=\"btn btn-xs perm-btn removePerm\"><span class=\"glyphicon glyphicon-minus\"></span></button>\n\
                    </div>"
                );
            }
        }
    }
    
    var permBox = "<div class=\"js-form-item form-item js-form-type-textfield form-type-textfield js-form-item-customurl form-item-customurl\">\n\
                        <input name=\"gluu_new_role[]\" placeholder=\"Input role name\" class=\"port form-text perm-btn\" style=\"display: inline; width: 200px !important; \" value=\"\" type=\"text\">\n\
                        <button type=\"button\" class=\"btn btn-xs perm-btn addPerm\"><span class=\"glyphicon glyphicon-plus\"></span></button>\n\
                        <button type=\"button\" class=\"btn btn-xs perm-btn removePerm\"><span class=\"glyphicon glyphicon-minus\"></span></button>\n\
                    </div>";
    
    $(".fieldset-wrapper").on('click','.addPerm',function(){
        $(this).parent().append(permBox);
    });
    
    $(".fieldset-wrapper").on('click','.removePerm',function(){
        $(this).parent().remove();
    });
    
    $("input[name='enrollment']").change(function(){
        if($(this).val() == 2){
            $(".perm-btn").removeAttr('disabled');
        }else{
            $(".perm-btn").prop("disabled",true);
        }
    });

})(jQuery);
