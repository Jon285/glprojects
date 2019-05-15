#[macro_use]
extern crate glium;
extern crate image;

use glium::glutin::{self, Api, GlProfile, GlRequest};
use glium::Surface;

use std::fs::File;
use std::io::prelude::*;
use std::io::Cursor;
use std::process;

#[derive(Clone, Copy)]
struct Vertex {
    pos: [f32; 2],
    texture_pos: [f32; 2],
}

implement_vertex!(Vertex, pos, texture_pos);

fn main() {
    //set basic state to use OpenGL
    let mut events_loop = glutin::EventsLoop::new();
    let window = glutin::WindowBuilder::new();
    let context = glutin::ContextBuilder::new()
        .with_gl(GlRequest::Specific(Api::OpenGl, (3, 3)))
        .with_gl_profile(GlProfile::Core);

    let display = glium::Display::new(window, context, &events_loop).unwrap_or_else(|err| {
        eprintln!("Error creating Display: {}", err);
        process::exit(1);
    });

    //loading texture image
    let text = image::load(
        Cursor::new(&include_bytes!("texture/texture.png")[..]),
        image::PNG,
    )
    .unwrap_or_else(|err| {
        eprintln!("Error opening texture file: {}", err);
        process::exit(1);
    })
    .to_rgba();

    //creating texture from image
    let text_dimensions = text.dimensions();
    let text =
        glium::texture::RawImage2d::from_raw_rgba_reversed(&text.into_raw(), text_dimensions);
    let texture = glium::texture::Texture2d::new(&display, text).unwrap_or_else(|err| {
        eprintln!("Failed to create the texture: {}", err);
        process::exit(1);
    });

    //create a Vec that holds the vertices of the triangle
    let vert1 = Vertex {
        pos: [-0.5, -0.5],
        texture_pos: [0.0, 0.0],
    };
    let vert2 = Vertex {
        pos: [0.0, 0.5],
        texture_pos: [0.0, 1.0],
    };
    let vert3 = Vertex {
        pos: [0.5, -0.25],
        texture_pos: [1.0, 0.0],
    };
    let triangle = vec![vert1, vert2, vert3];

    //vertex buffer created
    let vbo = glium::VertexBuffer::new(&display, &triangle).unwrap_or_else(|err| {
        eprintln!("Error creating Vertex Buffer: {}", err);
        process::exit(1);
    });

    //vertice indice created
    let indice = glium::index::NoIndices(glium::index::PrimitiveType::TrianglesList);

    let mut vshaders = File::open("vshader.glsl").unwrap();
    let mut fshaders = File::open("fshader.glsl").unwrap();

    let mut vs = String::new();
    let mut fs = String::new();

    vshaders.read_to_string(&mut vs).unwrap();
    fshaders.read_to_string(&mut fs).unwrap();

    //program compiled
    let program = glium::Program::from_source(&display, &vs, &fs, None).unwrap_or_else(|err| {
        eprintln!("Error compiling and/or linking shaders: {}", err);
        process::exit(1);
    });

    //helpful variables
    let mut f: f32 = 0.0;
    let mut closed = false;

    //main loop started
    while !closed {
        //update f
        f += 0.02;
        if f > 360.0 {
            f = 0.0;
        }

        //draw data to the buffer
        let mut target = display.draw();
        target.clear_color(0.0, 0.0, 1.0, 1.0);

        let uniforms = uniform! {
            matrix: [
                [f.cos(), 0.0, -f.sin(), 0.0],
                [0.0, 1.0, 0.0, 0.0],
                [f.sin(), 0.0, f.cos(), 0.0],
                [0.0, 0.0, 0.0, 1.0f32]
            ],
            tex: &texture,
        };

        target
            .draw(
                &vbo,      //buffer with the data
                &indice,   //indice on how to interpret the data
                &program,  //shaders to be used
                &uniforms, //variables to fetch
                &Default::default(),
            )
            .unwrap_or_else(|err| {
                eprintln!("Error drawing to the buffer: {}", err);
                process::exit(1);
            });

        //destroy buffer and draw data to the screen
        target.finish().unwrap_or_else(|err| {
            eprintln!("Error drawing to the screen: {}", err);
            process::exit(1);
        });

        //fetch events
        events_loop.poll_events(|ev| match ev {
            glutin::Event::WindowEvent { event, .. } => match event {
                glutin::WindowEvent::CloseRequested => closed = true,
                _ => (),
            },

            _ => (),
        });
    }
}
