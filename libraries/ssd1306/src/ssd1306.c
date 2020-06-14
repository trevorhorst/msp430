#include "ssd1306/ssd1306.h"

static const unsigned char config  [] = {0x3C, 0xAE};             // Display off
static const unsigned char config1 [] = {0x3C, 0xD5, 0x80 };       // set display clock division
static const unsigned char config2 [] = {0x3C, 0xA8, 0x3F };       // set multiplex
static const unsigned char config3 [] = {0x3C, 0xD3, 0x00 };       // set display offset
static const unsigned char config4 [] = {0x3C, 0x40 };             // set start line #0
static const unsigned char config5 [] = {0x3C, 0x8D, 0x14 };       // set charge pump
static const unsigned char config6 [] = {0x3C, 0x20, 0x00 };       // Memory mode
static const unsigned char config7 [] = {0x3C, 0xA1 };             // Segremap(0xA0 = reset, 0xA1 = 127 = SEG0)
static const unsigned char config8 [] = {0x3C, 0xC8 };             // Com scan dec (0xC0 = reset normal, 0xC8 = scan  from Com[n-1] - Com 0
static const unsigned char config9 [] = {0x3C, 0xDA, 0x12 };       // Set com pins
static const unsigned char config10[] = {0x3C, 0x81, 0xCF };       // Set contrast
static const unsigned char config11[] = {0x3C, 0xD9, 0xF1 };       // Set precharge
static const unsigned char config12[] = {0x3C, 0xDB, 0x40 };       // Set Vcom select
static const unsigned char config13[] = {0x3C, 0xA4 };             // Resume RAM content display
static const unsigned char config14[] = {0x3C, 0xA6 };             // Normal display not inverted
static const unsigned char config15[] = {0x3C, 0x00 };             // low col=0
static const unsigned char config16[] = {0x3C, 0x10 };             // high col=0
static const unsigned char config17[] = {0x3C, 0x40 };             // line #0
static const unsigned char config18[] = {0x3C, 0xAF };             // Display ON
static const unsigned char config19[] = {0x3C, 0x21, 0x00, 0x7F }; // Set column address; start 0, end 127
static const unsigned char config20[] = {0x3C, 0x22, 0x00, 0x07 }; // Set row address; start 0, end 7
static const unsigned char config21[] = {0x3C, 0xAF };             // Display ON
static const unsigned char config22[] = {0x3C, 0x21, 0x00, 0x7F};             // Display ON
static const unsigned char config23[] = {0x3C, 0x22, 0x00, 0x07};             // Display ON
static const unsigned char config24[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const unsigned char config25[] = {0x40, 0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00, 0x00, 0x00 };
//static const unsigned char config25[] = {0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

static struct i2c_data data   = {1+1, &config  };
static struct i2c_data data1  = {2+1, &config1 };
static struct i2c_data data2  = {2+1, &config2 };
static struct i2c_data data3  = {2+1, &config3 };
static struct i2c_data data4  = {1+1, &config4 };
static struct i2c_data data5  = {2+1, &config5 };
static struct i2c_data data6  = {2+1, &config6 };
static struct i2c_data data7  = {1+1, &config7 };
static struct i2c_data data8  = {1+1, &config8 };
static struct i2c_data data9  = {2+1, &config9 };
static struct i2c_data data10 = {2+1, &config10};
static struct i2c_data data11 = {2+1, &config11};
static struct i2c_data data12 = {2+1, &config12};
static struct i2c_data data13 = {1+1, &config13};
static struct i2c_data data14 = {1+1, &config14};
static struct i2c_data data15 = {1+1, &config15};
static struct i2c_data data16 = {1+1, &config16};
static struct i2c_data data17 = {1+1, &config17};
static struct i2c_data data18 = {1+1, &config18};
static struct i2c_data data19 = {3+1, &config19};
static struct i2c_data data20 = {3+1, &config20};
static struct i2c_data data21 = {1+1, &config21};
static struct i2c_data data22 = {3+1, &config22};
static struct i2c_data data23 = {3+1, &config23};
static struct i2c_data data24 = {9, &config24};
static struct i2c_data data25 = {9, &config25};

void ssd1306_init( struct i2c_device *dev )
{
    i2c_transfer( dev, &data);
    i2c_transfer( dev, &data1);
    i2c_transfer( dev, &data2);
    i2c_transfer( dev, &data3);
    i2c_transfer( dev, &data4);
    i2c_transfer( dev, &data5);
    i2c_transfer( dev, &data6);
    i2c_transfer( dev, &data7);
    i2c_transfer( dev, &data8);
    i2c_transfer( dev, &data9);
    i2c_transfer( dev, &data10);
    i2c_transfer( dev, &data11);
    i2c_transfer( dev, &data12);
    i2c_transfer( dev, &data13);
    i2c_transfer( dev, &data14);
    i2c_transfer( dev, &data15);
    i2c_transfer( dev, &data16);
    i2c_transfer( dev, &data17);
    i2c_transfer( dev, &data18);
    i2c_transfer( dev, &data19);
    i2c_transfer( dev, &data20);
    i2c_transfer( dev, &data21);


}

void ssd1306_write_char( const struct i2c_device *dev, struct i2c_data *data )
{
    i2c_transfer( dev, data );
}

void ssd1306_reset_cursor( const struct i2c_device *dev )
{
    // Reset column address; start 0 end 127
    i2c_transfer( dev, &data22 );
    // Set row address; start 0 end 7
    i2c_transfer( dev, &data23 );
}

void ssd1306_fill_screen( const struct i2c_device *dev )
{
    for( unsigned int i = 0; i < 8; i++ ) {
        for( unsigned int j = 0; j < 16; j++ ) {
            i2c_transfer(dev, &data25);
        }
    }
}

void ssd1306_clear_screen( const struct i2c_device *dev )
{
    for( unsigned int i = 0; i < 8; i++ ) {
        for( unsigned int j = 0; j < 16; j++ ) {
            i2c_transfer(dev, &data24);
        }
    }
}
