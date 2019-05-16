#[macro_use]
extern crate glium;

use glium::glutin::{self, Api, GlProfile, GlRequest};
use glium::Surface;

use std::fs::File;
use std::io::prelude::*;
use std::process;

use glteapot::*;

mod teapot;

fn main() {
    let mut events_loop = glutin::EventsLoop::new();
    let window = glutin::WindowBuilder::new();
    let context = glutin::ContextBuilder::new()
        .with_gl(GlRequest::Specific(Api::OpenGl, (3, 3)))
        .with_gl_profile(GlProfile::Core)
        .with_depth_buffer(24)
        .with_multisampling(8);
    let display = glium::Display::new(window, context, &events_loop).unwrap();

    let positions = glium::VertexBuffer::new(&display, &teapot::VERTICES).unwrap();
    let normals = glium::VertexBuffer::new(&display, &teapot::NORMALS).unwrap();
    let indices = glium::IndexBuffer::new(
        &display,
        glium::index::PrimitiveType::TrianglesList,
        &teapot::INDICES,
    )
    .unwrap();

    let mut v_file = File::open("shaders/vshader.glsl").unwrap_or_else(|err| {
        eprintln!("Failed to open vertex shader file: {}", err);
        process::exit(1);
    });

    let mut f_file = File::open("shaders/fshader.glsl").unwrap_or_else(|err| {
        eprintln!("Failed to open frag sahder file: {}", err);
        process::exit(1);
    });

    let mut v_source = String::new();
    let mut f_source = String::new();

    v_file.read_to_string(&mut v_source).unwrap_or_else(|err| {
        eprintln!("Failed to read vertex shader source: {}", err);
        process::exit(1);
    });

    f_file.read_to_string(&mut f_source).unwrap_or_else(|err| {
        eprintln!("Failed to read frag shader source: {}", err);
        process::exit(1);
    });

    let program = glium::Program::from_source(&display, &v_source, &f_source, None).unwrap();

    let mut closed = false;
    while !closed {
        let mut target = display.draw();
        target.clear_color_and_depth((0.0, 0.0, 0.0, 1.0), 1.0);

        let matrix = [
            [0.01, 0.0, 0.0, 0.0],
            [0.0, 0.01, 0.0, 0.0],
            [0.0, 0.0, 0.01, 0.0],
            [1.0, 0.2, 1.6, 1.0f32],
        ];

        let light = [1.4, 0.4, -0.7f32];

        let parameters = glium::DrawParameters {
            depth: glium::Depth {
                test: glium::draw_parameters::DepthTest::IfLess,
                write: true,
                ..Default::default()
            },
            ..Default::default()
        };

        let view = view_matrix(&[2.0, 1.0, 1.0], &[-2.0, -1.0, 1.0], &[0.0, 1.0, 0.0]);

        let perspective = {
            let (width, height) = target.get_dimensions();
            let aspect_ratio = height as f32 / width as f32;

            let fov: f32 = 3.141592 / 3.0;
            let zfar = 1024.0;
            let znear = 0.1;

            let f = 1.0 / (fov / 1.0).tan();

            [
                [f * aspect_ratio, 0.0, 0.0, 0.0],
                [0.0, f, 0.0, 0.0],
                [0.0, 0.0, (zfar + znear) / (zfar - znear), 1.0],
                [0.0, 0.0, -(2.0 * zfar * znear) / (zfar - znear), 0.0],
            ]
        };

        target
            .draw(
                (&positions, &normals),
                &indices,
                &program,
                &uniform! { matrix: matrix, perspective: perspective, view: view, u_light: light },
                &parameters,
            )
            .unwrap();
        target.finish().unwrap();

        events_loop.poll_events(|event| match event {
            glutin::Event::WindowEvent { event, .. } => match event {
                glutin::WindowEvent::CloseRequested => closed = true,
                _ => (),
            },
            _ => (),
        });
    }
}
