/**
 * Created by Vlad on 1/10/2016.
 */
jQuery(document).ready(function(){
    jQuery('#gluu-sso-scopes-scripts-setup').attr('enctype' , 'multipart/form-data');
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
            '</p>').appendTo(scntDiv);
        i++;
        return false;
    });

    jQuery('#remScnt').click( function() {
        console.log(jQuery('#remScnt').attr('type'));
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
                    '<input type="file" style="margin-right: 5px; margin-left: 3px; "  name="image_url_'+j+'" id="upload-btn_'+j+'" class="button-secondary" value="Upload app image" />' +

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

