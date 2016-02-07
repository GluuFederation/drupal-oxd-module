/**
 * Created by Vlad on 1/10/2016.
 */
function delete_scopes(val){
    jQuery.ajax({
        url: window.location,
        type: 'POST',
        data:{option:'oxd_openid_config_info_hidden', delete_scope:val},
        success: function(result){
            if(result){
                location.reload();
            }else{
                alert('Error, please try again.')
            }
        }});
}
jQuery(document).ready(function(){
    jQuery("#show_script_table").click(function(){
        jQuery("#custom_script_table").toggle();
    });
    jQuery("#show_scope_table").click(function(){
        jQuery("#custom_scope_table").toggle();
    });
});
jQuery(function() {
    var scntDiv = jQuery('#p_scents');
    var i = jQuery('#p_scents p').size() + 1;

    jQuery('#add_new_scope').click( function() {
        jQuery('<p><input type="text" name="new_scope[]" value="" class="form-text" size="60" maxlength="128" placeholder="Input scope name" />' +
            '<button style="margin-left: 10px" type="button" id="remScnt">Remove</button></p>').appendTo(scntDiv);
        i++;
        return false;
    });

    jQuery('#remScnt').click( function() {
        if( i > 2 ) {
            jQuery(this).parents('p').remove();
            i--;
        }
        return false;
    });

    var scntDiv_script = jQuery('#p_scents_script');
    var j = jQuery('#p_scents_script p').size() + 1;

    jQuery('#add_new_suctom_script').click( function() {
        jQuery('<p>' +
                    '<input type="text" style="margin-right: 5px " class="form-text" name="new_custom_script_name_'+j+'" size="30" placeholder="Input name (example Google+)" />' +
                    '<input type="text" style="margin-right: 5px; margin-left: 5px; " class="form-text" name="new_custom_script_value_'+j+'" size="40" placeholder="Input name in gluu server (example gplus)" />' +
                    '<input type="hidden" name="image_url_'+j+'" id="image_url_'+j+'" class="form-text" class="regular-text">'+
                    '<input type="button" style="margin-right: 5px; margin-left: 3px; "  name="upload-btn" onclick="upload_this('+j+')" id="upload-btn_'+j+'" class="button-secondary" value="Upload app image" />' +
                    '<button type="button" id="remScnt_script">Remove</button>' +
                '</p>').appendTo(scntDiv_script);
        j++;
        jQuery('#count_scripts').val(jQuery('#p_scents_script p').size());
        return false;
    });

    jQuery('#remScnt_script').click( function() {
        if( j > 2 ) {
            jQuery(this).parents('p').remove();
            j--;
            jQuery('#count_scripts').val(jQuery('#p_scents_script p').size());
        }
        return false;
    });
});

