import argparse
from moviepy import *
import os
import random
import torch
from TTS.api import TTS

video_end_buffer = 1.25

def generateSpeech(text, speech_path):
    device = "cuda"

    tts = TTS(model_name="tts_models/en/jenny/jenny", progress_bar=False).to(device)
    tts.tts_to_file(text=text, file_path=speech_path)

def getTrimmedClip(clip, target_duration):
    len_subclip = int(target_duration+video_end_buffer)

    subclip_end = random.randint(len_subclip, int(clip.duration))
    subclip_start = subclip_end-len_subclip

    return clip.subclip(subclip_start, subclip_end)

def getSpeechWithMusic(speech_path, music_path):
    music_intensity = 0.4
    speech_start_delay = 0.3
    fadein_time = str(speech_start_delay)

    speech = AudioFileClip(speech_path)
    music = getTrimmedClip(AudioFileClip(music_path), speech.duration).multiply_volume(music_intensity)

    composited_audio = CompositeAudioClip([speech.with_start(speech_start_delay), music])
    final_audio = composited_audio.audio_fadein(fadein_time)

    return final_audio

def getShortWithVideo(audio, video_path):
    gameplay = getTrimmedClip(VideoFileClip(video_path), audio.duration)
    short = gameplay.with_audio(audio)

    return short

def filesExist(*args):
    files_exist = True

    for path in args:
        if not os.path.exists(path):
            print(f"File does not exist: {path}")
            files_exist = False

    return files_exist

def generateShort(text, name):
    music_path = "resources/music.mp3"
    video_path = "resources/gameplay.mp4"
    if not filesExist(music_path, video_path):
        print("\nNecessary resources to build the short do not exist. Aborting.")
        return

    if not os.path.exists("out/"):
        os.mkdir("out")

    speech_path = "out/speech.mp3"
    output_path = f"out/{name}.mp4"

    generateSpeech(text, speech_path)
    audio = getSpeechWithMusic(speech_path, music_path)
    short = getShortWithVideo(audio, video_path)

    short.write_videofile(output_path)
    os.remove(speech_path)

def main():
    parser = argparse.ArgumentParser(description="Arguments that control the behaviour of the program as it generates a short.")
    parser.add_argument("-t", "--text", help="text to be put into shorts form; only use this if you are not using --file")
    parser.add_argument("-f", "--file", help="name of the file to read from; only use this if you are not using --text")
    parser.add_argument("-n", "--name", help="name of the output file; files are written as mp4 by default.")
    args = parser.parse_args()

    if args.file and args.text:
        print("Can't read from a file and a string at the same time. Try just one. Aborting.")
        return

    if not args.file and not args.text:
        print("No text to create a short from. Aborting.")
        return

    text = ""

    if args.file:
        text = open(args.file).read()
    elif args.text:
        text = args.text

    if args.name:
        name = args.name
    else:
        name = "short"

    generateShort(text, name)

if __name__ == "__main__":
    main()
