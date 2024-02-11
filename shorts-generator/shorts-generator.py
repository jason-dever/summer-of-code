from moviepy import *
import os
import random
import torch
from TTS.api import TTS

video_end_buffer = 1.25

def generateSpeech(content, speech_path):
    device = "cuda"

    tts = TTS(model_name="tts_models/en/jenny/jenny", progress_bar=False).to(device)
    tts.tts_to_file(text=content, file_path=speech_path)

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

def generateShort(content):
    music_path = "resources/music.mp3"
    video_path = "resources/gameplay.mp4"
    if not filesExist(music_path, video_path):
        print("\nNecessary resources to build the short do not exist. Aborting.")
        return

    if not os.path.exists("out/"):
        os.mkdir("out")

    speech_path = "out/speech.mp3"
    output_path = "out/short.mp4"

    generateSpeech(content, speech_path)
    audio = getSpeechWithMusic(speech_path, music_path)
    short = getShortWithVideo(audio, video_path)

    short.write_videofile(output_path)
    os.remove(speech_path)

content = "generated speech goes here"
generateShort(content)
