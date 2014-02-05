#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <string.h>

static int Index=0;
static void Gimp(char* filename)
{
	GError *Error=NULL;
	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_file(filename,&Error);
	if(NULL != pixbuf)
	{
		guchar *pixels, *p;
		int width, height, rowstride, n_channels;
		pixels=gdk_pixbuf_get_pixels(pixbuf);
		n_channels = gdk_pixbuf_get_n_channels (pixbuf);

		g_assert (gdk_pixbuf_get_colorspace (pixbuf) == GDK_COLORSPACE_RGB);
		g_assert (gdk_pixbuf_get_bits_per_sample (pixbuf) == 8);

		g_assert (!gdk_pixbuf_get_has_alpha (pixbuf));
		g_assert (n_channels == 3);

		width = gdk_pixbuf_get_width (pixbuf);
		height = gdk_pixbuf_get_height (pixbuf);

		rowstride = gdk_pixbuf_get_rowstride (pixbuf);
		pixels = gdk_pixbuf_get_pixels (pixbuf);

		printf("static const uint8_t IMG%d_pixels[]=\n{\n\t",Index);
		for(int y=0;y<height;y++)
		{
			for(int x=0;x<width;x++)
			{
				p = pixels + y * rowstride + x * n_channels;
				printf("%-3d,%-3d,%-3d,  ",p[0],p[1],p[2]);
			}
			printf("\n\t");
		}
		printf("\n};\n");
		printf("const SGIMPImage IMG%d_image=\n{/* %s */\n",Index,filename);
		printf("\t.dwWidth=%d,\n",width);
		printf("\t.dwHeight=%d,\n",height);
		printf("\t.dwBytes_per_pixel=%d,\n",3);
		printf("\t.pucPixel_data=IMG%d_pixels\n",Index);
		printf("};\n\n");

		Index++;
	}
	else
	{
		printf("Invalid arg: %s\n\tError:%s\n",filename,Error->message);
	}

}
static void Usage(void)
{
	printf("Usage:\n\tgimp xxx.jpg yyy.bmp\n");
}
int main(int argc,char* argv[])
{
	g_type_init();
	if(argc > 1)
	{
		for(int i=1;i<argc;i++)
		{
			Gimp(argv[i]);
		}
	}
	else
	{
		Usage();
	}
}
