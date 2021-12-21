struct VideoWindow : Window
{
    Settings& settings;
    Window& window;

    VerticalLayout layout;
    HorizontalLayout aa_layout, brightness_layout, contrast_layout;
    Label aa_label, brightness_label, contrast_label;
    LineEdit brightness_val, contrast_val;
    HorizontalSlider brightness_slider, contrast_slider; // gamma ?
    CheckButton aspect_checkbox, vsync_checkbox;
    FixedLayout exit_layout;
    Button exit_button;

    RadioButton aa_options[4];

    VideoWindow(Settings& s, Window& w) : settings(s), window(w)
    {
        setTitle("Video Settings");
        setResizable(false);

        layout.setMargin(10);
        
        aa_label.setText("Multisampling:");
        aa_options[0].setText("None");
        aa_options[1].setText("2x");
        aa_options[2].setText("4x");
        aa_options[3].setText("8x");

        aa_layout.append(aa_label, {0, 0}, 10);
        for(RadioButton& r : aa_options) aa_layout.append(r, {0, 0}, 10);
        RadioButton::group(aa_options[0], aa_options[1], aa_options[2], aa_options[3]);
        layout.append(aa_layout, {~0, 0}, 10);

        for(RadioButton& x : aa_options)
        {
            x.onActivate = [&] 
            {
                for(int i = 0; i < 4; i++)
                    if(aa_options[i].checked())
                        settings.video.multisampling = i;

                window.onSize();
                setFocused();
            };
        }

        /*brightness_label.setText("Brightness:");
        brightness_layout.append(brightness_label, {0, 0}, 10);
        
        brightness_val.setEditable(false);
        brightness_layout.append(brightness_val, {45, 0}, 10);

        brightness_slider.setLength(2001);
        brightness_slider.onChange = [&] 
        { 
            settings.video.brightness = brightness_slider.position();
            brightness_val.setText({settings.video.brightness / 10, "%"});
        };
        brightness_layout.append(brightness_slider, {~0, 30});
        layout.append(brightness_layout, {~0, 0}, 10);*/

        aspect_checkbox.setText("Keep Aspect Ratio");
        aspect_checkbox.onToggle = [&]
        { 
            settings.video.keep_aspect = aspect_checkbox.checked();
            window.onSize();
            setFocused();
        };
        layout.append(aspect_checkbox, {0, 0}, 10);

        vsync_checkbox.setText("Enable VSync");
        vsync_checkbox.onToggle = [&]
        { 
            settings.video.vsync = vsync_checkbox.checked();
            window.onSize();
            setFocused();
        };
        layout.append(vsync_checkbox, {0, 0}, 10);

        exit_button.setText("Exit");
        exit_button.onActivate = [&] 
        {
            setModal(false);
            setVisible(false);
        };
    }
    
    void create(const Position& pos)
    {
        setGeometry({pos.x, pos.y, 435, layout.minimumSize().height});
        
        if(settings.video.multisampling <= 3)
            aa_options[settings.video.multisampling].setChecked();

        aspect_checkbox.setChecked(settings.video.keep_aspect);
        vsync_checkbox.setChecked(settings.video.vsync);

        //brightness_slider.setPosition(settings.video.brightness);
        //brightness_val.setText({settings.video.brightness / 10, "%"});

        exit_layout.append(exit_button, {335, layout.minimumSize().height - exit_button.minimumSize().height - 10, 90, exit_button.minimumSize().height});
        append(layout);
        append(exit_layout);

        setVisible(true);
        //setModal(true);
    }
};

