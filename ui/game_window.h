#include "../chips/input.h"

struct GameWindow : Window
{
    HorizontalLayout layout;
    
    ListView game_view;
    
    struct TextLayout : HorizontalLayout
    {
        Widget spacer;
        Label label;

        TextLayout(unsigned font_size, int space, nall::string text)
        {
            if(font_size != 0) label.setFont(Font::sans(font_size, ""));
            label.setText(text);
            append(spacer, {space, 0});
            append(label, {0, 0});
        }
    };

    VerticalLayout side_layout;
    VerticalLayout control_layout;
    TextLayout control_label;

    std::vector<VerticalLayout*> game_layouts;

    HorizontalLayout button_layout;
    Button start_button;
    Button cancel_button;
    Label spacer;

    GameWindow() : control_label(11, 0, "Controls")
    {
        setTitle("New Game");
        setResizable(false);
        
        layout.setMargin(10);
        game_view.setHeaderText("Title", "Manufacturer", "Year");
        game_view.setHeaderVisible();

        for(const GameDesc& g : game_list)
        {
            game_view.append(g.name, g.manufacturer, g.year);
            
            std::vector<std::vector<TextLayout*>> player_layouts(1);
            player_layouts.back().push_back(new TextLayout(9, 20, "Coin / Start"));

            for(const InputDesc* input = g.desc->input; input != NULL && input->chip != NULL; input++)
            {
                int player = get_player(input->chip);
                while(player >= player_layouts.size())
                    player_layouts.push_back(std::vector<TextLayout*>(1, new TextLayout(9, 20, {"Player ", player_layouts.size()})));

                std::vector<TextLayout*>& current_player = player_layouts[player];
                current_player.push_back(new TextLayout(8, 40, {get_name(input->chip), 
                                                                get_pin_name(input->chip, input->pins), 
                                                                ": ", input->info}));
            }

            game_layouts.push_back(new VerticalLayout());

            // Rotate Coin/Start to end
            for(int i = 1; i <= player_layouts.size(); i++)
            {
                for(TextLayout* t : player_layouts[i % player_layouts.size()])
                    game_layouts.back()->append(*t, {0, 0}, 2);
            }
        }

        game_view.autoSizeColumns();

        start_button.setText("Start Game");
        cancel_button.setText("Cancel");

        button_layout.append(start_button, {100, 0});
        button_layout.append(spacer, {~0, 0});
        button_layout.append(cancel_button, {100, 0});

        layout.append(game_view, {300, ~0}, 10);
        
        side_layout.append(control_label, {0, 0}, 2);
        side_layout.append(control_layout, {~0, ~0});
        side_layout.append(button_layout, {~0, 0});

        game_view.onChange = [&]
        {
            for(VerticalLayout* x : game_layouts)
                control_layout.remove(*x);

            int current_config = game_view.selection();
            control_layout.append(*game_layouts[current_config], {~0, 0});
        };

        layout.append(side_layout, {~0, ~0});
        append(layout);

        game_view.onChange();
    }

    void create(const Position& pos)
    {
        setGeometry({pos.x, pos.y, 560, 360});

        game_view.setSelection(0);
        game_view.setSelected(true);
        game_view.onChange();
        
        setVisible(true);
        game_view.setFocused();
        setModal(true);
    }

    static int get_player(const ChipDesc* chip)
    {
        if(chip == chip_PADDLE1_HORIZONTAL_INPUT ||
           chip == chip_PADDLE1_VERTICAL_INPUT ||
           chip == chip_THROTTLE1_INPUT ||
           chip == chip_JOYSTICK1_INPUT ||
           chip == chip_WHEEL1_INPUT ||
           chip == chip_BUTTONS1_INPUT)
                return 1;
        if(chip == chip_PADDLE2_HORIZONTAL_INPUT ||
           chip == chip_PADDLE2_VERTICAL_INPUT ||
           chip == chip_JOYSTICK2_INPUT ||
           chip == chip_WHEEL2_INPUT ||
           chip == chip_BUTTONS2_INPUT)
                return 2;
        if(chip == chip_PADDLE3_HORIZONTAL_INPUT ||
           chip == chip_PADDLE3_VERTICAL_INPUT ||
           chip == chip_WHEEL3_INPUT ||
           chip == chip_BUTTONS3_INPUT)
                return 3;
        if(chip == chip_PADDLE4_HORIZONTAL_INPUT ||
           chip == chip_PADDLE4_VERTICAL_INPUT ||
           chip == chip_WHEEL4_INPUT ||
           chip == chip_BUTTONS4_INPUT)
                return 4;
        if(chip == chip_BUTTONS5_INPUT)
                return 5;
        if(chip == chip_BUTTONS6_INPUT)
                return 6;
        return 0;
    }

    static nall::string get_name(const ChipDesc* chip)
    {
        if(chip == chip_PADDLE1_HORIZONTAL_INPUT ||
           chip == chip_PADDLE1_VERTICAL_INPUT ||
           chip == chip_PADDLE2_HORIZONTAL_INPUT ||
           chip == chip_PADDLE2_VERTICAL_INPUT ||
           chip == chip_PADDLE3_HORIZONTAL_INPUT ||
           chip == chip_PADDLE3_VERTICAL_INPUT ||
           chip == chip_PADDLE4_HORIZONTAL_INPUT ||
           chip == chip_PADDLE4_VERTICAL_INPUT)
                return "Paddle";
        if(chip == chip_THROTTLE1_INPUT)
                return "Throttle";
        if(chip == chip_JOYSTICK1_INPUT ||
           chip == chip_JOYSTICK2_INPUT)
                return "Joystick";
        if(chip == chip_WHEEL1_INPUT ||
           chip == chip_WHEEL2_INPUT ||
           chip == chip_WHEEL3_INPUT ||
           chip == chip_WHEEL4_INPUT)
                return "Wheel";
        if(chip == chip_BUTTONS1_INPUT ||
           chip == chip_BUTTONS2_INPUT ||
           chip == chip_BUTTONS3_INPUT ||
           chip == chip_BUTTONS4_INPUT ||
           chip == chip_BUTTONS5_INPUT ||
           chip == chip_BUTTONS6_INPUT)
                return "Button";
        if(chip == chip_COIN_INPUT)
                return "Coin";
        if(chip == chip_START_INPUT)
                return "Start";
        
        return "";
    }

    static nall::string get_pin_name(const ChipDesc* chip, const std::array<uint8_t, 4>& pins)
    {
        int count = 0;
        while(count < pins.size() && pins[count] != 0) count++;
        
        nall::string s = "";

        if(chip == chip_JOYSTICK1_INPUT || chip == chip_JOYSTICK2_INPUT)
        {
            if(count == 0) return "";
            s = " ";
            for(unsigned i = 0; i < count; i++)
            {
                switch(pins[i])
                {
                    case Joystick::UP: s.append("Up"); break;
                    case Joystick::DOWN: s.append("Down"); break;
                    case Joystick::LEFT: s.append("Left"); break;
                    case Joystick::RIGHT: s.append("Right"); break;
                }
                if(i != count-1) s.append(", ");
            }
        }
        else if(chip == chip_BUTTONS1_INPUT || chip == chip_BUTTONS2_INPUT ||
                chip == chip_BUTTONS3_INPUT || chip == chip_BUTTONS4_INPUT ||
                chip == chip_BUTTONS5_INPUT || chip == chip_BUTTONS6_INPUT)
        {
            if(count == 0) return "s";
            s = (count == 1) ? "" : "s";
            s.append(" ", pins[0]);
            for(unsigned i = 1; i < count; i++) s.append(", ", pins[i]);
        }
        else if(chip == chip_COIN_INPUT || chip == chip_START_INPUT)
        {
            if(count == 0) return "";
            s.append(" ", pins[0]);
            for(unsigned i = 1; i < count; i++) s.append(", ", pins[i]);
        }
        return s;
    }
};
